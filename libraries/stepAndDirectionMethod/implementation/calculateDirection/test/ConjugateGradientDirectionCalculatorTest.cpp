#include "ConjugateGradientDirectionCalculator.h"
#include "ForwardModelMock.h"
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

        TEST(ConjugateGradientDirectionCalculatorTest, calculateDirectionTest)
        {
            // Create forwardmodel
            core::grid2D grid = getGrid();
            std::array<double, 2> xMin = {0.0, 0.0};
            std::array<double, 2> xMax = {2.0, 2.0};
            core::freqInfo freq;
            core::Sources sources(xMin, xMax, 2);
            core::Receivers receivers(xMin, xMax, 2);
            core::FrequenciesGroup frequencies(freq, 2000.0);

            forwardModels::ForwardModelInterface *forwardmodel = new forwardModels::ForwardModelMock(grid, sources, receivers, frequencies);

            // Create conjugate gradient direction calculator
            const double errorFunctionScalingFactor = 1.0;
            DirectionCalculator *directionCalulator = new ConjugateGradientDirectionCalculator(errorFunctionScalingFactor, forwardmodel);

            // Compute conjugate gradient direction
            core::dataGrid2D conjugateGradientDirection(grid);
            core::dataGrid2D chiEstimate(grid);
            std::vector<std::complex<double>> residuals(grid.getNumberOfGridPoints(), 1.0);
            conjugateGradientDirection = directionCalulator->calculateDirection(chiEstimate, residuals);

            // Compare conjugate gradient direction with expected value
            const int nrOfGridPoints = conjugateGradientDirection.getNumberOfGridPoints();
            const double expectedDirection = 5.0;

            const std::vector<double> &conjugateGradientDirectionData = conjugateGradientDirection.getData();
            for(int i = 0; i < nrOfGridPoints; i++)
            {
                ASSERT_DOUBLE_EQ(conjugateGradientDirectionData[i], expectedDirection);
            }

            delete forwardmodel;
            delete directionCalulator;
        }
    }   // namespace inversionMethods
}   // namespace fwi
