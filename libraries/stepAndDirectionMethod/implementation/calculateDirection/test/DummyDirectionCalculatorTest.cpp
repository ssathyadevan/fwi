#include "DummyDirectionCalculator.h"
#include "forwardmodelinterfacemock.h"
#include <gtest/gtest.h>

namespace fwi
{
    namespace inversionMethods
    {
        core::grid2D getGrid()
        {
            std::array<double, 2> xMin = {0.0, 0.0};
            std::array<double, 2> xMax = {2.0, 2.0};
            std::array<int, 2> nX = {2, 4};

            core::grid2D grid(xMin, xMax, nX);
            return grid;
        }

        TEST(DummyDirectionCalculatorTest, calculateDirectionTest)
        {
            // Arrange
            core::grid2D grid = getGrid();
            std::array<double, 2> xMin = {0.0, 0.0};
            std::array<double, 2> xMax = {2.0, 2.0};
            core::freqInfo freq(0.0, 10.0, 5);
            core::Sources sources(xMin, xMax, 2);
            core::Receivers receivers(xMin, xMax, 2);
            core::FrequenciesGroup frequencies(freq, 2000.0);

            forwardModels::forwardModelInterface *forwardModel;
            forwardModel = new forwardModels::ForwardModelInterfaceMock(grid, sources, receivers, frequencies);

            double errorFunctionalScalingFactor = 1.0;
            DirectionCalculator *directionCalculator = new DummyDirectionCalculator(errorFunctionalScalingFactor, forwardModel);
            core::dataGrid2D chiEstimate(grid);
            std::vector<std::complex<double>> residuals(grid.getNumberOfGridPoints(), 0.0);

            // Act
            std::vector<double> const dummyDirection = directionCalculator->calculateDirection(chiEstimate, residuals).getData();

            // Assert
            for(int i = 0; i < (int)dummyDirection.size(); i++)
            {
                ASSERT_DOUBLE_EQ(dummyDirection[i], 0.0);
            }

            delete forwardModel;
            delete directionCalculator;
        }
    }   // namespace inversionMethods
}   // namespace fwi
