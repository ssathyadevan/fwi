#include "ForwardModelMock.h"
#include <math.h>
namespace fwi
{
    namespace forwardModels
    {
        ForwardModelMock::ForwardModelMock(
            const core::grid2D &grid, const core::Sources &source, const core::Receivers &receiver, const core::FrequenciesGroup &freq)
            : _grid(grid)
            , _source(source)
            , _receiver(receiver)
            , _freq(freq)
            , _costFunction(CostFunction::leastSquares)
        {
            _kappaTimesResidualValue = 5.0;
            _residual.resize(freq.count * _source.count * _receiver.count);
        }

        void ForwardModelMock::calculatePData(const core::dataGrid2D &chiEst, std::vector<std::complex<double>> &pData)
        {
            const std::vector<double> &chiData = chiEst.getData();
            const double newValue = chiData[0];

            for(std::complex<double> &element : pData)
            {
                element = newValue;
            }
        }

        std::vector<std::complex<double>> &ForwardModelMock::calculateResidual(
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

        double ForwardModelMock::calculateCost(
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

        double ForwardModelMock::calculateLeastSquaresCost(
            std::vector<std::complex<double>> &residualArray, core::dataGrid2D &chiEstimate, const std::vector<std::complex<double>> &pData, double eta)
        {
            residualArray = calculateResidual(chiEstimate, pData);
            double residualNormSquared = calculateResidualNormSq(residualArray);
            double residual = eta * residualNormSquared;

            return residual;
        }

        double ForwardModelMock::calculateResidualNormSq(const std::vector<std::complex<double>> &residual)
        {
            double residualSq = normSq(residual, _freq.count * _source.count * _receiver.count);

            return residualSq;
        }

        const core::grid2D &ForwardModelMock::getGrid() { return _grid; }

        const core::Sources &ForwardModelMock::getSource() { return _source; }

        const core::Receivers &ForwardModelMock::getReceiver() { return _receiver; }

        const core::FrequenciesGroup &ForwardModelMock::getFreq() { return _freq; }

        CostFunction ForwardModelMock::getCostFunction() { return _costFunction; }

    }   // namespace forwardModels
}   // namespace fwi