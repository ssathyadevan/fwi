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



public:
    ForwardModelInterface(const grid_rect_2D &grid, const Sources_rect_2D &src, const Receivers_rect_2D &recv,
                          const Frequencies_group &freq, ProfileInterface &profiler, Input input)
        : m_grid(grid), m_src(src), m_recv(recv), m_freq(freq), input(input)
    {

    }

    virtual ~ForwardModelInterface()
    {

    }


    virtual void calculateData(std::complex<double> *p_data, volField_rect_2D_cpu chi, ConjGrad conjGrad) = 0;

    virtual void createTotalField1D(ConjGrad conjGrad, volField_rect_2D_cpu chi_est) = 0;

    const grid_rect_2D& get_m_grid() { return m_grid; }

    const Sources_rect_2D& get_m_src() { return m_src; }

    const Receivers_rect_2D& get_m_recv() {return m_recv;}

    const Frequencies_group& get_m_freq() {return m_freq;}

    virtual ProfileInterface& get_m_profiler() = 0;

    virtual Input getInput() = 0;

    virtual volComplexField_rect_2D_cpu** getKappa() = 0; // try to move/rename

    virtual void calculateKappa() = 0; // try to move/rename

    virtual void calculateResidual(volField_rect_2D_cpu chi_est, const std::complex<double> *const p_data) = 0;

    virtual std::complex<double>* getResidual() = 0;

    virtual double calculateResidualNormSq(double eta) = 0;

    virtual void calculateK_zeta(volField_rect_2D_cpu zeta) = 0; // try to move/rename

    virtual std::complex<double>* get_K_zeta() = 0; // try to move/rename


protected:
    const grid_rect_2D &m_grid; // pass where needed in main function
    const Sources_rect_2D &m_src; // pass where needed in main function
    const Receivers_rect_2D &m_recv; // pass where needed in main function
    const Frequencies_group &m_freq; // pass where needed in main function

    const Input input;

};

#endif

