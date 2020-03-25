#include "forwardModelInterface.h"
#include <math.h>

ForwardModelInterface::ForwardModelInterface(const Grid2D &grid, const Sources &src, const Receivers &recv,
                                             const FrequenciesGroup &freq)
    : /*_residual(), */_grid(grid), _src(src), _recv(recv), _freq(freq)
{
    _residual = std::vector<std::complex<double>>(_freq.nFreq * _src.nSrc * _recv.nRecv);
}

ForwardModelInterface::~ForwardModelInterface()
{
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

std::vector<std::complex<double>> &ForwardModelInterface::calculateResidual(const PressureFieldSerial &chiEst, const std::vector<std::complex<double>> &pDataRef)
{
    std::vector<std::complex<double>> pDataEst(_freq.nFreq * _recv.nRecv * _src.nSrc);

    calculatePData(chiEst, pDataEst);

    for (int i = 0; i < _freq.nFreq * _src.nSrc * _recv.nRecv; i++)
    {
        _residual[i] = pDataRef[i] - pDataEst[i];
        if (isnan(std::abs(_residual[i]))) {throw std::overflow_error("Residual is not a number");}
    }

    return _residual;
}

double ForwardModelInterface::calculateResidualNormSq(std::vector<std::complex<double>> &residual)
{
    double residualSq = normSq(residual, _freq.nFreq * _src.nSrc * _recv.nRecv);

    return residualSq;
}
