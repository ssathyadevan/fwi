#include "ForwardModelBase.h"
#include <math.h>

namespace fwi
{
    namespace forwardModels
    {
        ForwardModelBase::ForwardModelBase(
            const core::grid2D &grid, const core::Sources &source, const core::Receivers &receiver, const core::FrequenciesGroup &freq)
            : _grid(grid)
            , _source(source)
            , _receiver(receiver)
            , _freq(freq)
        {
            _residual = std::vector<std::complex<double>>(_freq.count * _source.count * _receiver.count);
        }

        ForwardModelBase::~ForwardModelBase() {}

        const core::grid2D &ForwardModelBase::getGrid() { return _grid; }

        const core::Sources &ForwardModelBase::getSource() { return _source; }

        const core::Receivers &ForwardModelBase::getReceiver() { return _receiver; }

        const core::FrequenciesGroup &ForwardModelBase::getFreq() { return _freq; }

        CostFunction ForwardModelBase::getCostFunction() { return _costFunction; }

        std::vector<std::complex<double>> &ForwardModelBase::calculateResidual(
            const core::dataGrid2D &chiEst, const std::vector<std::complex<double>> &pDataRef)
        {
            std::vector<std::complex<double>> pDataEst(_freq.count * _receiver.count * _source.count);

            calculatePData(chiEst, pDataEst);

            for(int i = 0; i < _freq.count * _source.count * _receiver.count; i++)
            {
                _residual[i] = pDataRef[i] - pDataEst[i];
                if(isnan(std::abs(_residual[i])))
                {
                    throw std::overflow_error("Residual is not a number");
                }
            }

            return _residual;
        }

        double ForwardModelBase::calculateResidualNormSq(const std::vector<std::complex<double>> &residual)
        {
            double residualSq = normSq(residual, _freq.count * _source.count * _receiver.count);

            return residualSq;
        }

        double ForwardModelBase::calculateCost(
            std::vector<std::complex<double>> &residualArray, core::dataGrid2D &chiEstimate, const std::vector<std::complex<double>> &pData, double eta)
        {
            double cost = 0.0;

            switch(_costFunction)
            {
                case leastSquares: cost = calculateLeastSquaresCost(residualArray, chiEstimate, pData, eta); break;
                default: throw std::invalid_argument("Invalid cost function selected.");
            }

            return cost;
        }

        double ForwardModelBase::calculateLeastSquaresCost(
            std::vector<std::complex<double>> &residualArray, core::dataGrid2D &chiEstimate, const std::vector<std::complex<double>> &pData, double eta)
        {
            residualArray = calculateResidual(chiEstimate, pData);
            double residualNormSquared = calculateResidualNormSq(residualArray);
            double residual = eta * residualNormSquared;

            return residual;
        }
    }   // namespace forwardModels
}   // namespace fwi
