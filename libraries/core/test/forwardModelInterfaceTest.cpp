#include <gtest/gtest.h>
#include "forwardmodelinterfacemock.h"

const Grid2D grid2D({0.0, 0.0}, {2.0, 2.0}, {4, 2});
const Sources src({0.0, 0.0}, {2.0, 2.0}, 8);
const Receivers recv({0.0, 0.0}, {2.0, 2.0}, 8);
const Freq freq = { 10, 40, 15 };
const FrequenciesGroup freqgroup(freq, 1.0);



TEST(forwardModelInterfaceTest, constructorTest)
{
    ForwardModelInterfaceMock forwardModelInterfaceMock(grid2D, src, recv, freqgroup);
    EXPECT_TRUE(grid2D == forwardModelInterfaceMock.getGrid());
}

TEST(forwardModelInterfaceTest, calculateResidualTest)
{
    ForwardModelInterfaceMock forwardModelInterfaceMock(grid2D, src, recv, freqgroup);
    std::vector<std::complex<double>> pDataRef(freqgroup.nFreq * recv.nRecv * src.nSrc);
    for(std::complex<double>& element : pDataRef)
    {
        element = 1.5;
    }


    PressureFieldSerial chiEst(grid2D);
    std::vector<std::complex<double>> residual;
    residual = forwardModelInterfaceMock.calculateResidual(chiEst, pDataRef);
    std::vector<std::complex<double>> expectedResidual(freqgroup.nFreq * recv.nRecv * src.nSrc);
    for(std::complex<double>& element : expectedResidual)
    {
        element = -0.5;
    }

    for (unsigned int i = 0; i < residual.size(); ++i)
    {
        EXPECT_NEAR(residual[i].imag(), expectedResidual[i].imag(), 0.001);
        EXPECT_NEAR(residual[i].real(), expectedResidual[i].real(), 0.001);

    }
}
