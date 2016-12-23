#ifndef CALCFIELD_H
#define CALCFIELD_H

#include <complex>
#include <vector>
#include <cassert>

#include "grid_rect_2D.h"
#include "input_parameters.h"
#include <Eigen/Dense>

using namespace Eigen;

template<typename T, template<typename> class volComplexField, template<typename> class volField, template<typename> class Green>
volComplexField<T> calcField(const Green<T> &G, const volField<T> &chi, const volComplexField<T> &p_init)
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

    matA.conservativeResize(n_cell, 1);
    b_f_rhs.conservativeResize(n_cell, 1);

    chi_p_old.Zero();
    T res = 0.0;

    p_tot = p_init;

    for(int it = 0; it < n_iter2; it++)
    {
        chi_p = p_tot * chi;

        dW = chi_p - chi_p_old;

        T dWNorm = dW.Norm();
        T chi_pNorm = chi_p.Norm();

        res = dWNorm / chi_pNorm;
        //std::cout << "Residual = " << res << std::endl;

        if(res < tol2 && it != 0)
        {
            std::cout << "Convergence after " << it << " iterations." << std::endl;
            break;
        }

        //calculate new phi
        if (calc_alpha == 1)
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
                p_tot += alpha[j]*phi[j];
        }
        else if(calc_alpha==0)
        {
            p_tot += G.ContractWithField(dW);
        }

        chi_p_old = chi_p;
        chi_p.Zero();
    }





    if(res >= tol2)
    {
        std::cout << "No convergence after " <<  n_iter2 << " iterations. Res = " << res << std::endl;
    }

    return p_tot;

}

#endif /* CALCFIELD_H */

