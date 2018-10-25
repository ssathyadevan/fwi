#ifndef CALCFIELD_H
#define CALCFIELD_H

#include <complex>
#include <vector>
#include <cassert>

#include "grid_rect_2D.h"
#include "input_parameters.h"
#include <eigen3/Eigen/Dense>
#include "ProfileCpu.h"
#include <string>

extern const int g_verbosity;

using namespace Eigen;

/*
    Babak 2018 10 24: This is the header file to do nonlinear field update on the domain equation
    by calculating p_tot from the follwoing equation IDs in the FWI_document:
    "incrementalContrastSrcs"
    "weightingFactorsField"
*/
template<typename T, template<typename> class volComplexField, template<typename> class volField, template<typename> class Green>
volComplexField<T> calcField(const Green<T> &G, const volField<T> &chi, const volComplexField<T> &p_init, const int &rank1)
{
    assert(&G.GetGrid() == &p_init.GetGrid());

    const grid_rect_2D<T> &m_grid = G.GetGrid();
    volComplexField<T> chi_p(m_grid), chi_p_old(m_grid);
    volComplexField<T> dW(m_grid), p_tot(m_grid), f_rhs(m_grid), matA_j(m_grid);

    int n_cell = m_grid.GetNumberOfGridPoints();

    std::vector< volComplexField<T> > phi;
    Matrix< std::complex<T>, Dynamic, Dynamic, ColMajor > matA;
    Matrix< std::complex<T>, Dynamic, 1, ColMajor > b_f_rhs;
    Matrix< std::complex<T>, Dynamic, 1, ColMajor > alpha;

    std::complex<T> *rhs_data, *matA_j_data;
    ProfileCpu profiler, prof2;

    matA.conservativeResize(n_cell, 1);
    b_f_rhs.conservativeResize(n_cell, 1);

    chi_p_old.Zero();// Babak 2018 10 25: initializing the chi_p (chi*p_tot);
    T res = 0.0;

    p_tot = p_init;//

    int rank = rank1;
    std::string name = "full iter " + std::to_string(rank);
    int rank_print = 0;

    for(int it = 0; it < n_iter2; it++)
    {

        chi_p = p_tot * chi;//Babak 2018 10 25: Equation ID: "incrementalContrastSrcs"

        dW = chi_p - chi_p_old;// Babak 2018 10 25: Equation ID: "incrementalContrastSrcs"

        T dWNorm = dW.Norm();
        T chi_pNorm = chi_p.Norm();

        res = dWNorm / chi_pNorm;//
        //std::cout << "Residual = " << res << std::endl;

        if(res < tol2 && it != 0)
        {
            if(rank1==rank_print){
		//std::string line_to_print = "Convergence after " + std::to_string(it) + "iterations. rank" + std::to_string(rank) + std::endl;
		std::string itstring = std::to_string(it);// + "iterations. rank" + std::to_string(rank) + std::endl;
//		std::string rankstring = std::to_string(rank);//std::cout << "Convergence after " << it << " iterations." << " rank " << rank << std::endl;
        std::string line_to_print = "Convergence after "+itstring+"iterations";
		std::cout << line_to_print << std::endl;
	    }
            break;
        }
        // Babak 2018 10 25: This part of the code is related to the Equation ID: "weightingFactorField"
        if (calc_alpha == 1) // Babak 2018 10 25: alpha (which is the step size of the update of chi in Equation ID: "contrastUpdate") is used
        {
            phi.push_back(G.ContractWithField(dW));
            f_rhs = G.ContractWithField(chi*p_init);

            rhs_data = f_rhs.GetDataPtr();  //calculate rhs

            matA_j = phi[it] - G.ContractWithField(chi*phi[it]);  //calculate new matrix column
            matA_j_data = matA_j.GetDataPtr();

            matA.conservativeResize(NoChange,it+1); //dynamic resizing of matrix matA
            alpha.conservativeResize(it+1);

            for(int i=0; i<n_cell; i++) //preparing the rhs and matA in the form required by the Eigen library to solve for alfa.
            {
                matA(i,it) = matA_j_data[i];
                b_f_rhs(i) = rhs_data[i];
            }

            alpha = matA.jacobiSvd(ComputeThinU | ComputeThinV).solve(b_f_rhs);

            p_tot = p_init;
            for(int j=0; j<it+1; j++)
                p_tot += alpha[j]*phi[j];// Babak 2018 10 25: "weightingFactorField"
        }
        else if(calc_alpha==0) // alpha is not used
        {
            //   profiler.StartRegion("contracting field");
            //p_tot += G.ContractWithField(dW);
            p_tot += G.dot1(dW);// Babak 2018 10 25: Equation ID: "buildField"  dot1 is coded in green_rect_2D_cpu.h
            //   profiler.EndRegion();
        }

        chi_p_old = chi_p; // updating the chi with the new chi
        chi_p.Zero();

    }
    //if (rank1==rank_print)
    //    prof2.EndRegion();




    if((res >= tol2) && (rank1==rank_print))
    {
        std::cout << "No convergence after " <<  n_iter2 << " iterations." << " rank " << rank << "Res = " << res << std::endl;
    }

    return p_tot;

}

#endif /* CALCFIELD_H */

