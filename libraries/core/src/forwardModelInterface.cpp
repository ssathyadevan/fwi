#include "forwardModelInterface.h"

ForwardModelInterface::ForwardModelInterface(const Grid2D &grid, const Sources &src, const Receivers &recv,
                                             const FrequenciesGroup &freq)
    : _residual(), _grid(grid), _src(src), _recv(recv), _freq(freq)
{
    _residual = new std::complex<double>[_freq.nFreq * _src.nSrc * _recv.nRecv];
}

ForwardModelInterface::~ForwardModelInterface()
{
    delete[] _residual;
    _residual = 0;
}

const Grid2D &ForwardModelInterface::getGrid()
{
    return _grid;
}

const Sources &ForwardModelInterface::getSrc()
{
    return _src;
}

const Receivers &ForwardModelInterface::getRecv()
{
    return _recv;
}

const FrequenciesGroup &ForwardModelInterface::getFreq()
{
    return _freq;
}

std::complex<double> *ForwardModelInterface::calculateResidual(const PressureFieldSerial &chiEst, const std::complex<double> *pDataRef)
{
    std::complex<double> pDataEst[_freq.nFreq * _recv.nRecv * _src.nSrc];

    calculatePData(chiEst, pDataEst);

    for (int i = 0; i < _freq.nFreq * _src.nSrc * _recv.nRecv; i++)
    {
        _residual[i] = pDataRef[i] - pDataEst[i];
    }

    return _residual;
}

double ForwardModelInterface::calculateResidualNormSq(std::complex<double> *residual)
{
    double residualSq = normSq(residual, _freq.nFreq * _src.nSrc * _recv.nRecv);

    return residualSq;
}
