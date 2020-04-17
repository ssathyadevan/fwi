#include "ConjugateGradientDirectionCalculator.h"
#include "forwardmodelinterfacemock.h"
#include <gtest/gtest.h>

grid2D getGrid()
{
    std::array<double, 2> xMin = {0.0, 0.0};
    std::array<double, 2> xMax = {2.0, 2.0};
    std::array<int, 2> nX = {2, 4};

    grid2D grid(xMin, xMax, nX);
    return grid;
}

TEST(ConjugateGradientDirectionCalculatorTest, calculateDirectionTest)
{
    // Create forwardmodel
    grid2D grid = getGrid();
    std::array<double, 2> xMin = {0.0, 0.0};
    std::array<double, 2> xMax = {2.0, 2.0};
    freqInfo freq;
    sources sources(xMin, xMax, 2);
    receivers receivers(xMin, xMax, 2);
    frequenciesGroup frequencies(freq, 2000.0);

    forwardModelInterface *forwardmodel = new ForwardModelInterfaceMock(grid, sources, receivers, frequencies);

    // Create conjugate gradient direction calculator
    const double errorFunctionScalingFactor = 1.0;
    DirectionCalculator *directionCalulator = new ConjugateGradientDirectionCalculator(errorFunctionScalingFactor, forwardmodel);

    // Compute conjugate gradient direction
    dataGrid2D cGDirection(grid);
    dataGrid2D chi(grid);
    std::vector<std::complex<double>> residuals(grid.getNumberOfGridPoints(), 1.0);
    cGDirection = directionCalulator->calculateDirection(chi, residuals);

    // Compare conjugate gradient direction with expected value
    const int nrOfGridPoints = cGDirection.getNumberOfGridPoints();
    const double expectedDirection = 5.0;

    const std::vector<double> &cGDirectionData = cGDirection.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(cGDirectionData[i], expectedDirection);
    }

    delete forwardmodel;
    delete directionCalulator;
}
