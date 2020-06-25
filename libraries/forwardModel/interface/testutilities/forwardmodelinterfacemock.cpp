#include "forwardmodelinterfacemock.h"

namespace fwi
{
    namespace forwardModels
    {
        ForwardModelInterfaceMock::ForwardModelInterfaceMock(
            const core::grid2D &grid, const core::Sources &source, const core::Receivers &receiver, const core::FrequenciesGroup &freq)
            : forwardModelInterface(grid, source, receiver, freq)
        {
            _kappaTimesResidualValue = 5.0;
        }

        void ForwardModelInterfaceMock::calculatePData(const core::dataGrid2D &chiEst, std::vector<std::complex<double>> &pData)
        {
            const std::vector<double> &chiData = chiEst.getData();
            const double newValue = chiData[0];

            for(std::complex<double> &element : pData)
            {
                element = newValue;
            }
        }
    }   // namespace forwardModels
}   // namespace fwi
