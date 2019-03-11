#ifndef FORWARDMODELINTERFACE_H
#define FORWARDMODELINTERFACE_H

#include "forwardModelInput.h"
#include "pressureFieldSerial.h"
#include "pressureFieldComplexSerial.h"
#include "grid2D.h"
#include "frequenciesGroup.h"
#include "sources.h"
#include "receivers.h"

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
        :_residual(), _grid(grid), _src(src), _recv(recv), _freq(freq), _fmInput(fmInput)
    {
        _residual = new std::complex<double>[_freq.nFreq * _src.nSrc * _recv.nRecv];
    }

    virtual ~ForwardModelInterface()
    {
        delete[] _residual;
        _residual = 0;
    }

    const grid2D& getGrid();
    const sources& getSrc();
    const receivers& getRecv();
    const frequenciesGroup& getFreq();

    const forwardModelInput& getForwardModelInput();

    virtual void calculatePdataEst(const pressureFieldSerial &chiEst, std::complex<double> *pData) = 0;
    virtual void getPdataEst(const pressureFieldSerial &chiEst, std::complex<double> *pData) = 0;

    std::complex<double>* calculateResidual(const pressureFieldSerial &chiEst, const std::complex<double> *Pdata);
    double calculateResidualNormSq(std::complex<double> *residual);

private:

    std::complex<double> *_residual;

protected:

    const grid2D            &_grid;
    const sources           &_src;
    const receivers         &_recv;
    const frequenciesGroup  &_freq;
    const forwardModelInput &_fmInput;

};

#endif



