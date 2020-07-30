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
        {
            _kappaTimesResidualValue = 5.0;
            _residual.resize(freq.count * _source.count * _receiver.count);
        }

        std::vector<std::complex<double>> ForwardModelMock::calculatePData(const core::dataGrid2D &chiEst)
        {
            return std::vector<std::complex<double>>(_freq.count * _source.count * _receiver.count, (chiEst.getData())[0]);
        }

        std::vector<std::complex<double>> &ForwardModelMock::calculateResidual(
            const core::dataGrid2D &chiEst, const std::vector<std::complex<double>> &pDataRef)
        {
            std::vector<std::complex<double>> pDataEst = calculatePData(chiEst);

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

        const core::grid2D &ForwardModelMock::getGrid() { return _grid; }

        const core::Sources &ForwardModelMock::getSource() { return _source; }

        const core::Receivers &ForwardModelMock::getReceiver() { return _receiver; }

        const core::FrequenciesGroup &ForwardModelMock::getFreq() { return _freq; }

    }   // namespace forwardModels
}   // namespace fwi
