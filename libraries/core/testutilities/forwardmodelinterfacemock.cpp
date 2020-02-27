#include "forwardmodelinterfacemock.h"

ForwardModelInterfaceMock::ForwardModelInterfaceMock(const Grid2D &grid, const Sources &src, const Receivers &recv,
                                                     const FrequenciesGroup &freq)
    : ForwardModelInterface(grid, src, recv, freq)
{

}

void ForwardModelInterfaceMock::calculatePData(const PressureFieldSerial &chiEst, std::vector<std::complex<double>> &pData)
{
    chiEst.GetGrid();

    for(std::complex<double>& element : pData)
    {
        element = 2;
    }

}
