#ifndef FORWARDMODELINTERFACE_H
#define FORWARDMODELINTERFACE_H

#include "frequenciesGroup.h"
#include "greensFunctions.h"
#include "calcField.h"
#include "forwardModelInput.h"
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
                          const frequenciesGroup &freq, genericInput gInput, forwardModelInput fmInput)
        : grid(grid), src(src), recv(recv), freq(freq), gInput(gInput),fmInput(fmInput)
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

    virtual forwardModelInput getInput() = 0;

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
    const genericInput gInput;
    const forwardModelInput fmInput;
};

#endif

