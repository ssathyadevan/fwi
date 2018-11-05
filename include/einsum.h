#ifndef EINSUM_H
#define EINSUM_H

#include <iostream>
#include <complex>
#include "volField_rect_2D_cpu.h"
#include "sources_rect_2D.h"
#include "receivers_rect_2D.h"
#include "grid_rect_2D.h"
#include "greens_rect_2D_cpu.h"
#include <volComplexField_rect_2D_cpu.h>
#include "mpi.h"
#include <frequencies_group.h>

//Babak 2018 10 29: Get rid of templates of grid_rect_2D class

// Babak and Saurabh 2019 10 30: REmoving template from volField and replace with volField_rect_cpu
class einsum
{
    //const grid_rect_2D<double> &m_grid;// Babak 2018 10 29: Get rid of templates
    const grid_rect_2D &m_grid;
    const Sources_rect_2D &m_src;
    const Receivers_rect_2D &m_recv;
    const Frequencies_group &m_freq;

    const int &m_n_freq;
    const int &m_n_src;
    const int &m_n_recv;

    public:
    //        einsum(const grid_rect_2D &grid, const Sources_rect_2D<double> &src, const Receivers_rect_2D<double> &recv, const Frequencies_group &freq)
    //        : m_grid(grid), m_src(src), m_recv(recv), m_freq(freq), m_n_freq(m_freq.nFreq), m_n_src(m_src.nSrc), m_n_recv(m_recv.nRecv)
    //        {
    //        } // Babak 2018 10 29: get rid of template for grid_rect_2D
        einsum(const grid_rect_2D &grid, const Sources_rect_2D &src, const Receivers_rect_2D &recv, const Frequencies_group &freq)
        : m_grid(grid), m_src(src), m_recv(recv), m_freq(freq), m_n_freq(m_freq.nFreq), m_n_src(m_src.nSrc), m_n_recv(m_recv.nRecv)
        {
        }

        einsum(const einsum &) = delete; //delete the copy constructor to forbid copying of objects of this class
        einsum & operator=(const einsum &) = delete;  //delete the assignment constructor to forbid copying of objects of this class


        void einsum_Gr_Pest(volComplexField_rect_2D_cpu **Kappa, const Greens_rect_2D_cpu *const *green, const volComplexField_rect_2D_cpu *const *P_est) const
        {
            int l_i, l_j;

            for (int i = 0; i < m_n_freq; i++)
            {
                l_i = i*m_n_recv*m_n_src;
                for (int j = 0; j < m_n_recv; j++)
                {
                    l_j = j*m_n_src;
                    for(int k = 0; k < m_n_src; k++)
                    {
                        *Kappa[l_i + l_j + k] = ( *green[i]->GetReceiverCont(j) ) * (*P_est[i*m_n_src + k]);
                    }
                }
            }
        }

        void einsum_Gr_Pest(volComplexField_rect_2D_cpu **Kappa, const Greens_rect_2D_cpu *const *green, const volComplexField_rect_2D_cpu *const *const *P_est) const
        {
            int l_i, l_j;

            for (int i = 0; i < m_n_freq; i++)
            {
                l_i = i*m_n_recv*m_n_src;
                for (int j = 0; j < m_n_recv; j++)
                {
                    l_j = j*m_n_src;
                    for(int k = 0; k < m_n_src; k++)
                    {
                        *Kappa[l_i + l_j + k] = ( *green[i]->GetReceiverCont(j) ) * (*P_est[i][k]);
                    }
                }
            }
        }

        void einsum_K_zeta(const volComplexField_rect_2D_cpu *const *Kappa, const volField_rect_2D_cpu &chi_est, std::complex<double> *K_zeta) const
        {
            for (int i = 0; i < m_n_freq*m_n_recv*m_n_src; i++)
            {
                K_zeta[i] = Summation( *Kappa[i], chi_est);
            }
        }


        void einsum_K_res(const volComplexField_rect_2D_cpu *const *Kappa, const std::complex<double> *res, volComplexField_rect_2D_cpu &K_res) const
        {
            int l_i, l_j;
           // volComplexField_rect_2D_cpu K_res(m_grid);
            K_res.Zero();
            volComplexField_rect_2D_cpu K_dummy(m_grid);

            for (int i = 0; i < m_n_freq; i++)
            {
                l_i = i*m_n_recv*m_n_src;

                for (int j = 0; j < m_n_recv; j++)
                {
                    l_j = j*m_n_src;

                    for(int k = 0; k < m_n_src; k++)
                    {
                        K_dummy = *Kappa[l_i + l_j + k];
                        K_dummy.Conjugate(); //take conjugate of elements of Kappa (required for algorithm einsum('ijkl,ijk->l',conk(K),r) )
                        K_res += K_dummy * res[l_i + l_j + k];
  //                      std::cout << *K_res.GetDataPtr() << "   " << *K_dummy.GetDataPtr()* res[l_i + l_j + k] << std::endl;
  //                      std::cout << *K_res.GetDataPtr() << "   " << *K_dummy.GetDataPtr()* res[l_i + l_j + k] << std::endl;
                    }
                }
            }
/*          if ( sizeof(T) == sizeof(double) )
            {
                //std::cout << "hello" << m_grid.GetNumberOfGridPoints() << std::endl;
                MPI::COMM_WORLD.Allreduce(MPI_IN_PLACE, K_res.GetDataPtr(), m_grid.GetNumberOfGridPoints(), MPI::DOUBLE_COMPLEX, MPI_SUM);
            }
            else
                MPI::COMM_WORLD.Allreduce(MPI_IN_PLACE, K_res.GetDataPtr(), m_grid.GetNumberOfGridPoints(), MPI::COMPLEX, MPI_SUM);
*/      }


        std::complex<double> einsum_dot(const std::complex<double> *x, const std::complex<double> *y) const
        {
            std::complex<double> dot = 0;
            int l_i, l_j;

            for (int i = 0; i < m_n_freq; i++)
            {
                l_i = i*m_n_recv*m_n_src;

                for (int j = 0; j < m_n_recv; j++)
                {
                    l_j = j*m_n_src;

                    for(int k = 0; k < m_n_src; k++)
                    {
                        dot += conj(x[l_i + l_j + k]) * y[l_i + l_j + k];
                    }
                }
            }
            return dot;
        }



 //       virtual ~einsum();
    protected:
    private:
};

#endif // EINSUM_H
