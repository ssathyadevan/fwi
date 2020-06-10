#include "forwardModelInterface.h"
#include <math.h>

forwardModelInterface::forwardModelInterface(const grid2D &grid, const sources &src, const receivers &recv, const frequenciesGroup &freq) :
    /*_residual(), */ _grid(grid), _src(src), _recv(recv), _freq(freq)
{
    _residual = std::vector<std::complex<double>>(_freq.nFreq * _src.nSrc * _recv.nRecv);
}

forwardModelInterface::~forwardModelInterface() {}

const grid2D &forwardModelInterface::getGrid() { return _grid; }

const sources &forwardModelInterface::getSrc() { return _src; }

const receivers &forwardModelInterface::getRecv() { return _recv; }

const frequenciesGroup &forwardModelInterface::getFreq() { return _freq; }

std::vector<std::complex<double>> &forwardModelInterface::calculateResidual(const dataGrid2D &chiEst, const std::vector<std::complex<double>> &pDataRef)
{
    std::vector<std::complex<double>> pDataEst(_freq.nFreq * _recv.nRecv * _src.nSrc);

    calculatePData(chiEst, pDataEst);

    for(int i = 0; i < _freq.nFreq * _src.nSrc * _recv.nRecv; i++)
    {
        _residual[i] = pDataRef[i] - pDataEst[i];
        if(isnan(std::abs(_residual[i])))
        {
            throw std::overflow_error("Residual is not a number");
        }
    }

    return _residual;
}

double forwardModelInterface::calculateResidualNormSq(const std::vector<std::complex<double>> &residual)
{
    double residualSq = normSq(residual, _freq.nFreq * _src.nSrc * _recv.nRecv);

    return residualSq;
}

double forwardModelInterface::calculateCost(
    std::vector<std::complex<double>> &residualArray, dataGrid2D &chiEstimate, const std::vector<std::complex<double>> &pData, double eta)
{
    residualArray = calculateResidual(chiEstimate, pData);
    double residualNormSquared = calculateResidualNormSq(residualArray);
    double residual = eta * residualNormSquared;

    return residual;
}
