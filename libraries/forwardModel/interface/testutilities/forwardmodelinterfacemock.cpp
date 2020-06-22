#include "forwardmodelinterfacemock.h"

ForwardModelInterfaceMock::ForwardModelInterfaceMock(const core::grid2D &grid, const core::sources &src, const core::receivers &recv, const core::frequenciesGroup &freq) :
    forwardModelInterface(grid, src, recv, freq)
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
