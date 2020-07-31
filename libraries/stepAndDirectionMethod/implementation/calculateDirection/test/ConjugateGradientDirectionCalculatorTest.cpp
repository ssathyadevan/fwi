#include "ConjugateGradientDirectionCalculator.h"
#include "ForwardModelMock.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace fwi
{
    namespace inversionMethods
    {
        class ConjugateGradientDirectionCalculatorTest : public ::testing::Test
        {
        protected:
            std::array<double, 2> xMin = {0.0, 0.0};
            std::array<double, 2> xMax = {2.0, 2.0};
            std::array<int, 2> nX = {2, 4};
            core::freqInfo freq;

            const double errorFunctionScalingFactor = 1.0;
        };

        TEST_F(ConjugateGradientDirectionCalculatorTest, calculateDirection)
        {
            // Arrange
            core::grid2D grid(xMin, xMax, nX);
            core::Sources sources(xMin, xMax, 2);
            core::Receivers receivers(xMin, xMax, 2);
            core::FrequenciesGroup frequencies(freq, 2000.0);

            forwardModels::ForwardModelInterface *forwardModel = new forwardModels::ForwardModelMock(grid, sources, receivers, frequencies);

            ConjugateGradientDirectionCalculator directionCalculator(errorFunctionScalingFactor, forwardModel);

            core::dataGrid2D chiEstimate(grid);
            std::vector<std::complex<double>> residual(chiEstimate.getNumberOfGridPoints(), 1.0);

            // Act
            std::vector<double> conjugateGradientDirectionData = directionCalculator.calculateDirection(chiEstimate, residual).getData();

            const double expectedDirection = 5.0;

            for(int it = 0; it < chiEstimate.getNumberOfGridPoints(); ++it)
            {
                ASSERT_DOUBLE_EQ(conjugateGradientDirectionData[it], expectedDirection);
            }

            delete forwardModel;
        }
    }   // namespace inversionMethods
}   // namespace fwi
