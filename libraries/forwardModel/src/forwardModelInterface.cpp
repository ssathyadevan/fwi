#include "forwardModelInterface.h"

ForwardModelInterface::ForwardModelInterface(const grid2D &grid, const sources &src, const receivers &recv,
                      const frequenciesGroup &freq, const forwardModelInput &fmInput)
    :_residual(), _grid(grid), _src(src), _recv(recv), _freq(freq), _fmInput(fmInput)
{
    _residual = new std::complex<double>[_freq.nFreq * _src.nSrc * _recv.nRecv];
}

virtual ForwardModelInterface::~ForwardModelInterface()
{
    delete[] _residual;
    _residual = 0;
}


const grid2D& ForwardModelInterface::getGrid()
{
    return _grid;
}

const sources& ForwardModelInterface::getSrc()
{
    return _src;
}

const receivers& ForwardModelInterface::getRecv()
{
    return _recv;
}

const frequenciesGroup& ForwardModelInterface::getFreq()
{
    return _freq;
}

const forwardModelInput& ForwardModelInterface::getForwardModelInput()
{
    return _fmInput;
}

std::complex<double>* ForwardModelInterface::calculateResidual(const pressureFieldSerial &chiEst, const std::complex<double> *Pdata)
{
    std::complex<double>* PdataEst = new std::complex<double>[_freq.nFreq * _recv.nRecv * _src.nSrc];

    getPdataEst(chiEst, PdataEst);

    for (int i = 0; i < _freq.nFreq * _src.nSrc * _recv.nRecv; i++)
    {
        _residual[i] = Pdata[i] - PdataEst[i];
    }

    delete[] PdataEst;

    return _residual;
}

double ForwardModelInterface::calculateResidualNormSq(std::complex<double> *residual)
{
    double residualSq = normSq(residual, _freq.nFreq * _src.nSrc * _recv.nRecv);

    return residualSq;
}




