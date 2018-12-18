#ifndef FORWARDMODELINTERFACE_H
#define FORWARDMODELINTERFACE_H

#include "frequencies_group.h"
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
                          const Frequencies_group &freq, Input input)
        : grid(grid), src(src), recv(recv), freq(freq), input(input)
    {

    }

    virtual ~ForwardModelInterface()
    {
    }

    virtual void calculateData(std::complex<double> *p_data, volField_rect_2D_cpu chi, Iter2 conjGrad) = 0;

    virtual void createTotalField1D(Iter2 conjGrad, volField_rect_2D_cpu chi_est) = 0;

    const grid_rect_2D& getGrid();

    const Sources_rect_2D& getSrc();

    const Receivers_rect_2D& getRecv();

    const Frequencies_group& getFreq();

    virtual void calculateKRes(volComplexField_rect_2D_cpu &kRes) = 0;

    virtual Input getInput() = 0;

    virtual void intermediateForwardModelStep1() = 0;

    virtual void calculateResidual(volField_rect_2D_cpu chi_est, const std::complex<double> *const p_data) = 0;

    virtual std::complex<double>* getResidual() = 0;

    virtual double calculateResidualNormSq(double eta) = 0;

    virtual std::complex<double>* intermediateForwardModelStep2(volField_rect_2D_cpu zeta) = 0;

protected:
    const grid_rect_2D& grid;
    const Sources_rect_2D& src;
    const Receivers_rect_2D& recv;
    const Frequencies_group& freq;

    const Input input;
};

#endif

