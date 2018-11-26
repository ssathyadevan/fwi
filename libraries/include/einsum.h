#ifndef EINSUM_H
#define EINSUM_H

#include <iostream>
#include <complex>
#include "volField_rect_2D_cpu.h"
#include "sources_rect_2D.h"
#include "receivers_rect_2D.h"
#include "grid_rect_2D.h"
#include "greens_rect_2D_cpu.h"
#include "variable_structure.h"
#include <volComplexField_rect_2D_cpu.h>
#include <frequencies_group.h>


class einsum
{
    const grid_rect_2D &m_grid;
    const Sources_rect_2D &m_src;
    const Receivers_rect_2D &m_recv;
    const Frequencies_group &m_freq;

    const int &m_n_freq;
    const int &m_n_src;
    const int &m_n_recv;

    public:
        einsum(const grid_rect_2D &grid, const Sources_rect_2D &src, const Receivers_rect_2D &recv, const Frequencies_group &freq);

        einsum(const einsum &) = delete; //delete the copy constructor to forbid copying of objects of this class
        einsum & operator=(const einsum &) = delete;  //delete the assignment constructor to forbid copying of objects of this class


        void einsum_Gr_Pest(volComplexField_rect_2D_cpu **Kappa, const Greens_rect_2D_cpu *const *green, const volComplexField_rect_2D_cpu *const *P_est) const;

        void einsum_Gr_Pest(volComplexField_rect_2D_cpu **Kappa, const Greens_rect_2D_cpu *const *green, const volComplexField_rect_2D_cpu *const *const *P_est) const;

        void einsum_K_zeta(const volComplexField_rect_2D_cpu *const *Kappa, const volField_rect_2D_cpu &chi_est, std::complex<double> *K_zeta) const;

        void einsum_K_res(const volComplexField_rect_2D_cpu *const *Kappa, const std::complex<double> *res, volComplexField_rect_2D_cpu &K_res) const;

        std::complex<double> einsum_dot(const std::complex<double> *x, const std::complex<double> *y) const;

};

#endif // EINSUM_H
