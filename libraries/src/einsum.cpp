#include "einsum.h"

einsum::einsum(const grid_rect_2D &grid, const Sources_rect_2D &src, const Receivers_rect_2D &recv, const Frequencies_group &freq)
    : m_grid(grid), m_src(src), m_recv(recv), m_freq(freq), m_n_freq(m_freq.nFreq), m_n_src(m_src.nSrc), m_n_recv(m_recv.nRecv)
{
}


//void einsum::einsum_Gr_Pest(volComplexField_rect_2D_cpu **Kappa, const Greens_rect_2D_cpu *const *green, const volComplexField_rect_2D_cpu *const *P_est) const
//{
//    int l_i, l_j;

//    for (int i = 0; i < m_n_freq; i++)
//    {
//        l_i = i*m_n_recv*m_n_src;
//        for (int j = 0; j < m_n_recv; j++)
//        {
//            l_j = j*m_n_src;
//            for(int k = 0; k < m_n_src; k++)
//            {
//                *Kappa[l_i + l_j + k] = ( *green[i]->GetReceiverCont(j) ) * (*P_est[i*m_n_src + k]);
//            }
//        }
//    }
//}

//void einsum::einsum_Gr_Pest(volComplexField_rect_2D_cpu **Kappa, const Greens_rect_2D_cpu *const *green, const volComplexField_rect_2D_cpu *const *const *P_est) const
//{
//    int l_i, l_j;

//    for (int i = 0; i < m_n_freq; i++)
//    {
//        l_i = i*m_n_recv*m_n_src;
//        for (int j = 0; j < m_n_recv; j++)
//        {
//            l_j = j*m_n_src;
//            for(int k = 0; k < m_n_src; k++)
//            {
//                *Kappa[l_i + l_j + k] = ( *green[i]->GetReceiverCont(j) ) * (*P_est[i][k]);
//            }
//        }
//    }
//}

//void einsum::einsum_K_zeta(const volComplexField_rect_2D_cpu *const *Kappa, const volField_rect_2D_cpu &chi_est, std::complex<double> *K_zeta) const
//{
//    for (int i = 0; i < m_n_freq*m_n_recv*m_n_src; i++)
//    {
//        K_zeta[i] = Summation( *Kappa[i], chi_est);
//    }
//}


void einsum::einsum_K_res(const volComplexField_rect_2D_cpu *const *Kappa, const std::complex<double> *res, volComplexField_rect_2D_cpu &K_res) const
{
    int l_i, l_j;
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

            }
        }
    }
}


//std::complex<double> einsum::einsum_dot(const std::complex<double> *x, const std::complex<double> *y) const
//{
//    std::complex<double> dot = 0;
//    int l_i, l_j;

//    for (int i = 0; i < m_n_freq; i++)
//    {
//        l_i = i*m_n_recv*m_n_src;

//        for (int j = 0; j < m_n_recv; j++)
//        {
//            l_j = j*m_n_src;

//            for(int k = 0; k < m_n_src; k++)
//            {
//                dot += conj(x[l_i + l_j + k]) * y[l_i + l_j + k];
//            }
//        }
//    }
//    return dot;
//}




