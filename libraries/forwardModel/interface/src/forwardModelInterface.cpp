#include "forwardModelInterface.h"
#include <math.h>

namespace fwi
{
    namespace forwardModels
    {
        forwardModelInterface::forwardModelInterface(
            const core::grid2D &grid, const core::Sources &source, const core::Receivers &receiver, const core::FrequenciesGroup &freq)
            : /*_residual(), */ _grid(grid)
            , _source(source)
            , _receiver(receiver)
            , _freq(freq)
        {
            _residual = std::vector<std::complex<double>>(_freq.nFreq * _source.count * _receiver.count);
        }

        forwardModelInterface::~forwardModelInterface() {}

        const core::grid2D &forwardModelInterface::getGrid() { return _grid; }
        // Review: usually the get functions follow literally the name of the variables. Update to getSource
        const core::Sources &forwardModelInterface::getSrc() { return _source; }
        // Review: update to getReceiver
        const core::Receivers &forwardModelInterface::getRecv() { return _receiver; }

        const core::FrequenciesGroup &forwardModelInterface::getFreq() { return _freq; }

        std::vector<std::complex<double>> &forwardModelInterface::calculateResidual(
            const core::dataGrid2D &chiEst, const std::vector<std::complex<double>> &pDataRef)
        {
            std::vector<std::complex<double>> pDataEst(_freq.nFreq * _receiver.count * _source.count);

            calculatePData(chiEst, pDataEst);

            for(int i = 0; i < _freq.nFreq * _source.count * _receiver.count; i++)
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
            double residualSq = normSq(residual, _freq.nFreq * _source.count * _receiver.count);

            return residualSq;
        }

        double forwardModelInterface::calculateCost(
            std::vector<std::complex<double>> &residualArray, core::dataGrid2D &chiEstimate, const std::vector<std::complex<double>> &pData, double eta)
        {
            costFunction = leastSquares;
            double cost;

            switch(costFunction)
            {
                case leastSquares: cost = calculateLeastSquaresCost(residualArray, chiEstimate, pData, eta); break;
                default: std::cout << "Invalid cost function selected." << std::endl;
            }

            return cost;
        }

        double forwardModelInterface::calculateLeastSquaresCost(
            std::vector<std::complex<double>> &residualArray, core::dataGrid2D &chiEstimate, const std::vector<std::complex<double>> &pData, double eta)
        {
            residualArray = calculateResidual(chiEstimate, pData);
            double residualNormSquared = calculateResidualNormSq(residualArray);
            double residual = eta * residualNormSquared;

            return residual;
        }
    }   // namespace forwardModels
}   // namespace fwi
