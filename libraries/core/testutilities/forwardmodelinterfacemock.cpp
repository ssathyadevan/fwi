#include "forwardmodelinterfacemock.h"

ForwardModelInterfaceMock::ForwardModelInterfaceMock(const grid2D &grid, const sources &src, const receivers &recv, const frequenciesGroup &freq) :
    forwardModelInterface(grid, src, recv, freq)
{
}

void ForwardModelInterfaceMock::calculatePData(const dataGrid2D &chiEst, std::vector<std::complex<double>> &pData)
{
    const std::vector<double> &chiData = chiEst.getData();
    const double newValue = chiData[0];

    for(std::complex<double> &element : pData)
    {
        element = newValue;
    }
}
