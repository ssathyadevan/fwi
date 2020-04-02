#include "forwardmodelinterfacemock.h"

ForwardModelInterfaceMock::ForwardModelInterfaceMock(const grid2D &grid, const sources &src, const receivers &recv,
                                                     const frequenciesGroup &freq)
    :
    forwardModelInterface(grid, src, recv, freq)
{

}

void ForwardModelInterfaceMock::calculatePData(const pressureFieldSerial &chiEst, std::vector<std::complex<double>> &pData)
{
    (void)chiEst;

    for(std::complex<double>& element : pData)
    {
        element = 2;
    }

}
