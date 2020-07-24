#include "ForwardModelMock.h"
#include <gtest/gtest.h>

namespace fwi
{
    namespace forwardModels
    {
        const core::grid2D grid2D({0.0, 0.0}, {2.0, 2.0}, {4, 2});
        const core::Sources source({0.0, 0.0}, {2.0, 2.0}, 8);
        const core::Receivers receiver({0.0, 0.0}, {2.0, 2.0}, 8);
        const core::freqInfo freq = {10, 40, 15};
        const core::FrequenciesGroup freqGroup(freq, 1.0);

        TEST(forwardModelInterfaceTest, constructorTest)
        {
            ForwardModelMock forwardModelMock(grid2D, source, receiver, freqGroup);
            EXPECT_TRUE(grid2D == forwardModelMock.getGrid());
        }

        TEST(forwardModelInterfaceTest, calculateResidualTest)
        {
            // Given
            ForwardModelMock forwardModelMock(grid2D, source, receiver, freqGroup);
            std::vector<std::complex<double>> pDataRef(freqGroup.count * receiver.count * source.count);
            for(std::complex<double> &element : pDataRef)
            {
                element = 1.5;
            }

            // When
            core::dataGrid2D chiEst(grid2D);
            chiEst = 2.0;
            std::vector<std::complex<double>> residual;
            residual = forwardModelMock.calculateResidual(chiEst, pDataRef);

            // Then
            std::vector<std::complex<double>> expectedResidual(freqGroup.count * receiver.count * source.count);
            for(std::complex<double> &element : expectedResidual)
            {
                element = -0.5;
            }
            for(unsigned int i = 0; i < residual.size(); ++i)
            {
                EXPECT_NEAR(residual[i].imag(), expectedResidual[i].imag(), 0.001);
                EXPECT_NEAR(residual[i].real(), expectedResidual[i].real(), 0.001);
            }
        }

        TEST(forwardModelInterfaceTest, calculateResidualNormSqTest)
        {
            // Given
            ForwardModelMock forwardModelMock(grid2D, source, receiver, freqGroup);
            std::vector<std::complex<double>> pDataRef(freqGroup.count * receiver.count * source.count);
            for(std::complex<double> &element : pDataRef)
            {
                element = 1;
            }
            core::dataGrid2D chiEst(grid2D);
            std::vector<std::complex<double>> residual;

            // When
            residual = forwardModelMock.calculateResidual(chiEst, pDataRef);
            double residualSq = forwardModelMock.calculateResidualNormSq(residual);
            double expectedResidualSq = residual.size();

            // Then
            EXPECT_NEAR(residualSq, expectedResidualSq, 0.001);
        }
    }   // namespace forwardModels
}   // namespace fwi
