#ifndef FORWARDMODELINTERFACE_H
#define FORWARDMODELINTERFACE_H


#include "frequencies_group.h"
#include "volField_rect_2D_cpu.h"
#include "ProfileInterface.h"
#include "GreensFunctions.h"
#include "calcField.h"

#include <complex>


class ForwardModelInterface
{
protected:
    const grid_rect_2D &m_grid;
    const Sources_rect_2D &m_src;
    const Receivers_rect_2D &m_recv;
    const Frequencies_group &m_freq;

    Greens_rect_2D_cpu **m_greens;
    volComplexField_rect_2D_cpu ***p_0;
    volComplexField_rect_2D_cpu ***p_tot;

    volField_rect_2D_cpu m_chi;
    ProfileInterface &m_profiler;

    const int m_nfreq;
    const int m_nrecv;
    const int m_nsrc;

public:
    ForwardModelInterface(const grid_rect_2D &grid, const Sources_rect_2D &src, const Receivers_rect_2D &recv, const Frequencies_group &freq, ProfileInterface &profiler, const volField_rect_2D_cpu chi)
        : m_grid(grid), m_src(src), m_recv(recv), m_freq(freq), m_greens(), p_0(), p_tot(), m_chi(m_grid), m_profiler(profiler), m_nfreq(m_freq.nFreq), m_nrecv(m_recv.nRecv), m_nsrc(m_src.nSrc)
    {
    }

    ~ForwardModelInterface()
    {
    }

    virtual void createGreens() = 0;

    virtual void deleteGreens() = 0;

    virtual void SetBackground(const volField_rect_2D_cpu &chi_) = 0;

    virtual void createP0() = 0;

    virtual void deleteP0() = 0;

    virtual void deleteTotalField() = 0;

    virtual void calculateData(std::complex<double> *p_data) = 0;

    virtual void createTotalField(ConjGrad conjGrad) = 0;

};

#endif

