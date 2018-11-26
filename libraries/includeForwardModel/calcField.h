#ifndef CALCFIELD_H
#define CALCFIELD_H

#include "grid_rect_2D.h"
#include <eigen3/Eigen/Dense>
#include "ProfileCpu.h"
#include "greens_rect_2D_cpu.h"
#include "volComplexField_rect_2D_cpu.h"
#include "variable_structure.h"

#include <complex>
#include <vector>
#include <cassert>
#include <string>

extern const int g_verbosity;

using namespace Eigen;

/*
    Babak 2018 10 24: This is the header file to do nonlinear field update on the domain equation
    by calculating p_tot from the following equation IDs in the FWI_document:
    "incrementalContrastSrcs"
    "weightingFactorsField"
*/
inline volComplexField_rect_2D_cpu calcField(const Greens_rect_2D_cpu &G, const volField_rect_2D_cpu &chi, const volComplexField_rect_2D_cpu &p_init, ConjGrad conjGrad)
{
    assert(&G.GetGrid() == &p_init.GetGrid());

    const grid_rect_2D &m_grid = G.GetGrid();

    volComplexField_rect_2D_cpu chi_p(m_grid), chi_p_old(m_grid);
    volComplexField_rect_2D_cpu dW(m_grid), p_tot(m_grid), f_rhs(m_grid), matA_j(m_grid);

    int n_cell = m_grid.GetNumberOfGridPoints();

    std::vector< volComplexField_rect_2D_cpu > phi;
    Matrix< std::complex<double> , Dynamic, Dynamic, ColMajor > matA;
    Matrix< std::complex<double> , Dynamic, 1, ColMajor > b_f_rhs;
    Matrix< std::complex<double> , Dynamic, 1, ColMajor > alpha;

    std::complex<double>  *rhs_data, *matA_j_data;
    ProfileCpu profiler, prof2;

    matA.conservativeResize(n_cell, 1);
    b_f_rhs.conservativeResize(n_cell, 1);

    chi_p_old.Zero();// Babak 2018 10 25: initializing the chi_p (chi*p_tot);
    double res = 0.0;

    p_tot = p_init;//


    for(int it = 0; it < conjGrad.nIter; it++)
    {

        chi_p = p_tot * chi;//Babak 2018 10 25: Equation ID: "incrementalContrastSrcs"

        dW = chi_p - chi_p_old;// Babak 2018 10 25: Equation ID: "incrementalContrastSrcs"

        double dWNorm = dW.Norm();
        double chi_pNorm = chi_p.Norm();

        res = dWNorm / chi_pNorm;//
        //std::cout << "Residual = " << res << std::endl;

        if(res < conjGrad.tolerance && it != 0)
        {
            if(true){
                std::string itstring = std::to_string(it);
                std::string line_to_print = "Convergence after "+itstring+"iterations";
                std::cout << line_to_print << std::endl;
            }
            break;
        }
        // Babak 2018 10 25: This part of the code is related to the Equation ID: "weightingFactorField"
        if (conjGrad.calcAlpha)
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
        else
        {
            //   profiler.StartRegion("contracting field");
            //p_tot += G.ContractWithField(dW);
            p_tot += G.dot1(dW);// Babak 2018 10 25: Equation ID: "weightingFactorField"  dot1 is coded in green_rect_2D_cpu.h
            //   profiler.EndRegion();
        }

        chi_p_old = chi_p; // updating the chi with the new chi
        chi_p.Zero();

    }



    if(res >= conjGrad.tolerance)
    {
        std::cout << "No convergence after " <<  conjGrad.nIter << " iterations." << "Res = " << res << std::endl;
    }

    return p_tot;

}

#endif /* CALCFIELD_H */
