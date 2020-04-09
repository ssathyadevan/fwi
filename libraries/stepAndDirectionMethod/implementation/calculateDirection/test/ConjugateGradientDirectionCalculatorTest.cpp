#include "ConjugateGradientDirectionCalculator.h"
#include <gtest/gtest.h>

grid2D getGrid()
{
    std::array<double, 2> xMin = {0.0, 0.0};
    std::array<double, 2> xMax = {2.0, 2.0};
    std::array<int, 2> nX = {2, 4};

    grid2D grid(xMin, xMax, nX);
    return grid;
}

TEST(conjugateGradientDirectionCalculatorTest, calculateDirectionTest)
{
    grid2D grid = getGrid();
    double errorFunctionScalingFactor = 1.0;
    dataGrid2D chi(grid);
    complexDataGrid2D residuals(grid);

    DirectionCalculator *directionCalulator = new ConjugateGradientDirectionCalculator(grid, errorFunctionScalingFactor);
    dataGrid2D cGDirection(grid);
    cGDirection = directionCalulator->calculateDirection(chi, residuals);

    const int nrOfGridPoints = cGDirection.getNumberOfGridPoints();
    const std::vector<double> &data = cGDirection.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], 1.0);
    }

    delete directionCalulator;
}
