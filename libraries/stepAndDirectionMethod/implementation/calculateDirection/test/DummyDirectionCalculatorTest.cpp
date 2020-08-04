#include "DummyDirectionCalculator.h"
#include "ForwardModelMock.h"
#include <gtest/gtest.h>

namespace fwi
{
    namespace inversionMethods
    {
        /**
         * @brief This test can be used as a base to create the unit test for any customized DirectionCalculator
         */
        TEST(DummyDirectionCalculatorTest, calculateDirectionTest)
        {
            //            // Arrange
            //            const std::array<double, 2> xMin = {0.0, 0.0};
            //            const std::array<double, 2> xMax = {2.0, 2.0};
            //            const std::array<int, 2> nX = {2, 4};

            //            const core::grid2D grid(xMin, xMax, nX);

            //            const core::freqInfo freq(0.0, 10.0, 5);
            //            const core::Sources sources(xMin, xMax, 2);
            //            const core::Receivers receivers(xMin, xMax, 2);
            //            const core::FrequenciesGroup frequencies(freq, 2000.0);

            //            forwardModels::ForwardModelInterface *const forwardModel = new forwardModels::ForwardModelMock(grid, sources, receivers, frequencies);
            //            const double errorFunctionalScalingFactor = 1.0;
            //            DirectionCalculator *const directionCalculator = new DummyDirectionCalculator(errorFunctionalScalingFactor, forwardModel);
            //            const core::dataGrid2D chiEstimate(grid);
            //            const std::vector<std::complex<double>> residuals(grid.getNumberOfGridPoints(), 0.0);

            //            // Fill the expectedDirection vector with the output you expect from the directionCalculator->calculateDirection() method
            //            const std::vector<double> expectedDirection(grid.getNumberOfGridPoints(), 0.0);

            //            // Act
            //            std::vector<double> const dummyDirection = directionCalculator->calculateDirection(chiEstimate, residuals).getData();

            //            // Assert
            //            for(int it = 0; it < (int)dummyDirection.size(); ++it)
            //            {
            //                // if the values differ by small truncament errors, consider EXPECT_NEAR(value1, value2, tolerance);
            //                ASSERT_DOUBLE_EQ(dummyDirection[it], expectedDirection[it]);
            //            }

            //            delete forwardModel;
            //            delete directionCalculator;
        }
    }   // namespace inversionMethods
}   // namespace fwi
