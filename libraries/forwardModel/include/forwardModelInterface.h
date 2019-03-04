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
                          const frequenciesGroup &freq, const forwardModelInput &fmInput)
        : m_grid(grid), m_src(src), m_recv(recv), m_freq(freq), m_fmInput(fmInput)
    {

    }

    virtual ~ForwardModelInterface()
    {

    }

    const grid2D& getGrid();

    const sources& getSrc();

    const receivers& getRecv();

    const frequenciesGroup& getFreq();

    const forwardModelInput& getForwardModelInput();

    virtual void initializeForwardModel() = 0;

    virtual std::complex<double>* createPdataEst(const pressureFieldSerial &chiEst) = 0;

    std::complex<double>* getResidual(const pressureFieldSerial &chiEst, const std::complex<double> *Pdata);
    double getResidualNormSq(const pressureFieldSerial &chiEst, const std::complex<double> *Pdata);

private:

    std::complex<double> *m_residual;
    std::complex<double> *m_residualSq;

    void calculateResidual(const pressureFieldSerial &chiEst, const std::complex<double> *Pdata);
    void calculateResidualNormSq(const pressureFieldSerial &chiEst, const std::complex<double> *Pdata);

protected:

    const grid2D& m_grid;
    const sources& m_src;
    const receivers& m_recv;
    const frequenciesGroup& m_freq;
    const forwardModelInput& m_fmInput;
};

#endif

//    virtual void calculateData(std::complex<double> *p_data, pressureFieldSerial chi, Iter2 conjGrad) = 0;

//    virtual void createTotalField1D(pressureFieldSerial chi_est) = 0;

//    virtual void calculateKRes(pressureFieldComplexSerial& kRes) = 0;

//    virtual void intermediateForwardModelStep1() = 0;

//    virtual void calculateResidual(pressureFieldSerial chi_est, const std::complex<double> *const p_data) = 0;

//    virtual std::complex<double>* getResidual() = 0;

//    virtual double calculateResidualNormSq(double eta) = 0;

//    virtual std::complex<double>* intermediateForwardModelStep2(pressureFieldSerial zeta) = 0;

