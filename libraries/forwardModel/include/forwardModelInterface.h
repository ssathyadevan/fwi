#ifndef FORWARDMODELINTERFACE_H
#define FORWARDMODELINTERFACE_H

#include "frequenciesGroup.h"
#include "greensFunctions.h"
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
    ForwardModelInterface(const grid2D &grid, const sources &src, const receivers &recv,
                          const frequenciesGroup &freq, Input input)
        : grid(grid), src(src), recv(recv), freq(freq), input(input)
    {

    }

    virtual ~ForwardModelInterface()
    {
    }

    virtual void calculateData(std::complex<double> *p_data, pressureFieldSerial chi, Iter2 conjGrad) = 0;

    virtual void createTotalField1D(Iter2 conjGrad, pressureFieldSerial chi_est) = 0;

    const grid2D& getGrid();

    const sources& getSrc();

    const receivers& getRecv();

    const frequenciesGroup& getFreq();

    virtual void calculateKRes(pressureFieldComplexSerial &kRes) = 0;

    virtual Input getInput() = 0;

    virtual void intermediateForwardModelStep1() = 0;

    virtual void calculateResidual(pressureFieldSerial chi_est, const std::complex<double> *const p_data) = 0;

    virtual std::complex<double>* getResidual() = 0;

    virtual double calculateResidualNormSq(double eta) = 0;

    virtual std::complex<double>* intermediateForwardModelStep2(pressureFieldSerial zeta) = 0;

protected:
    const grid2D& grid;
    const sources& src;
    const receivers& recv;
    const frequenciesGroup& freq;

    const Input input;
};

#endif

