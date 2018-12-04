#ifndef FORWARDMODELINTERFACE_H
#define FORWARDMODELINTERFACE_H


#include "frequencies_group.h"
#include "volField_rect_2D_cpu.h"
#include "ProfileInterface.h"
#include "GreensFunctions.h"
#include "calcField.h"

#include <complex>

inline double normSq(const std::complex<double> *data, int n)
{
    double result = double(0.0);
    for(int i=0; i<n; i++)
    {
        result += std::norm(data[i]);
    }
    return result;
}

inline double normSq(std::complex<double> *data, int n)
{
    double result = double(0.0);
    for(int i=0; i<n; i++)
    {
        result += std::norm(data[i]);
    }
    return result;
}

class ForwardModelInterface
{
protected:
    const grid_rect_2D &m_grid; // pass where needed in main function
    const Sources_rect_2D &m_src; // pass where needed in main function
    const Receivers_rect_2D &m_recv; // pass where needed in main function
    const Frequencies_group &m_freq; // pass where needed in main function

    Greens_rect_2D_cpu **m_greens; // belongs in the forward model, but don't know how to implement
    volComplexField_rect_2D_cpu ***p_0; // belongs in the interface
    volComplexField_rect_2D_cpu ***p_tot; // belongs in the interface

    ProfileInterface &m_profiler; // pass where needed in main function

    const int m_nfreq;// read from input
    const int m_nrecv;// read from input
    const int m_nsrc;// read from input
    const int n_total;// read from input

    volComplexField_rect_2D_cpu **Kappa; // greens*p_tot in eq:ModelDataGreensConv
    volComplexField_rect_2D_cpu **p_tot1D; // p_tot stored as a 1D array
    std::complex<double>* residual; // residual for the loop with input.iter1.n
    std::vector<std::complex<double>> res_first_it; // normsquare of above residual (each element
    // of this vector is the residual corresponding to that iteration in the iter1 loop)
    std::complex<double> *K_zeta ;

public:
    ForwardModelInterface(const grid_rect_2D &grid, const Sources_rect_2D &src, const Receivers_rect_2D &recv, const Frequencies_group &freq, ProfileInterface &profiler)
        : m_grid(grid), m_src(src), m_recv(recv), m_freq(freq), m_greens(), p_0(), p_tot(), m_profiler(profiler), m_nfreq(m_freq.nFreq),
          m_nrecv(m_recv.nRecv), m_nsrc(m_src.nSrc), n_total(m_freq.nFreq*m_recv.nRecv*m_src.nSrc)
    {
        residual = new std::complex<double>[m_nfreq*m_nrecv*m_nsrc];
        K_zeta = new std::complex<double>[m_nfreq*m_nrecv*m_nsrc];
    }

    ~ForwardModelInterface()
    {
        for (int i = 0; i < m_nfreq*m_nrecv*m_nsrc; i++)
            delete Kappa[i];
        delete[] Kappa;
        Kappa = nullptr;

        for (int i = 0; i < m_nfreq*m_nsrc; i++)
            delete p_tot1D[i];
        delete[] p_tot1D;
        p_tot1D = nullptr;
    }


    virtual void createGreens() = 0;

    virtual void deleteGreens() = 0;

    virtual void createP0() = 0;

    virtual void deleteP0() = 0;

    virtual void deleteTotalField() = 0;

    virtual void calculateData(std::complex<double> *p_data, volField_rect_2D_cpu chi) = 0;

    virtual void createTotalField(ConjGrad conjGrad, volField_rect_2D_cpu chi) = 0;

    virtual void createTotalField1D(ConjGrad conjGrad, volField_rect_2D_cpu chi_est) = 0;

    virtual const grid_rect_2D& get_m_grid() = 0;

    virtual const Sources_rect_2D& get_m_src() = 0;

    virtual const Receivers_rect_2D& get_m_recv() = 0;

    virtual const Frequencies_group& get_m_freq() = 0;

    virtual ProfileInterface& get_m_profiler() = 0;

    virtual const int get_m_nfreq() = 0;

    virtual const int get_m_nrecv() = 0;

    virtual const int get_m_nsrc() = 0;

    virtual volComplexField_rect_2D_cpu*** get_p_0() = 0;

    virtual Greens_rect_2D_cpu** get_m_greens() = 0;

    virtual volComplexField_rect_2D_cpu** getKappa() = 0;

    virtual void calculateKappa() = 0;

    virtual void calculateResidual(volField_rect_2D_cpu chi_est, const std::complex<double> *const p_data) = 0;

    virtual std::complex<double>* get_residual() = 0;

    virtual double calculateResidualNormSq(double eta) = 0;

    virtual void calculateK_zeta(volField_rect_2D_cpu zeta) = 0;

    virtual std::complex<double>* get_K_zeta() = 0;


};

#endif

