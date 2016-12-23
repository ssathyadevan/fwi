#ifndef EINSUM_H
#define EINSUM_H

#include <iostream>
#include <complex>

#include "sources_rect_2D.h"
#include "receivers_rect_2D.h"
#include "frequencies.h"
#include "grid_rect_2D.h"



template<typename T, template<typename> class volComplexField, template<typename> class volField, template<typename> class Greens>
class einsum
{
    const grid_rect_2D<T> &m_grid;
    const Sources_rect_2D<T> &m_src;
    const Receivers_rect_2D<T> &m_recv;
    const Frequencies<T> &m_freq;

    const int &m_n_freq;
    const int &m_n_src;
    const int &m_n_recv;

    public:
        einsum(const grid_rect_2D<T> &grid, const Sources_rect_2D<T> &src, const Receivers_rect_2D<T> &recv, const Frequencies<T> &freq)
        : m_grid(grid), m_src(src), m_recv(recv), m_freq(freq), m_n_freq(m_freq.nFreq), m_n_src(m_src.nSrc), m_n_recv(m_recv.nRecv)
        {
        }

 //       einsum(const einsum<T,volComplexField,volField> &) = delete; //delete the copy constructor to forbid copying of objects of this class
 //       einsum<T,volComplexField,volField> & operator=(const einsum<T,volComplexField,volField> &) = delete;  //delete the assignment constructor to forbid copying of objects of this class


        void einsum_Gr_Pest(volComplexField<T> **Kappa, const Greens<T> *const *green, const volComplexField<T> *const *P_est) const
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


        void einsum_K_zeta(const volComplexField<T> *const *Kappa, const volField<T> &chi_est, std::complex<T> *K_zeta) const
        {
            for (int i = 0; i < m_n_freq*m_n_recv*m_n_src; i++)
            {
                K_zeta[i] = Summation( *Kappa[i], chi_est);
            }
        }


        void einsum_K_res(const volComplexField<T> *const *Kappa, const std::complex<T> *res, volComplexField<T> &K_res) const
        {
            int l_i, l_j;

            K_res = T(0.0);
            volComplexField<T> K_dummy(m_grid);

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
        }


        std::complex<T> einsum_dot(const std::complex<T> *x, const std::complex<T> *y) const
        {
            std::complex<T> dot = 0;
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
