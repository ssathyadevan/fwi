#include "DirectionCalculator.h"
#include "DirectionCalculatorMock.h"
#include <gtest/gtest.h>

grid2D getGrid()
{
    std::array<double, 2> xMin = {0.0, 0.0};
    std::array<double, 2> xMax = {2.0, 2.0};
    std::array<int, 2> nX = {2, 4};

    grid2D grid(xMin, xMax, nX);
    return grid;
}

TEST(DirectionCalculatorTest, ConstructorGridTest)
{
    grid2D grid = getGrid();
    double errorFunctionalScalingFactor = 1.0;

    DirectionCalculator *directionCalulator = new DirectionCalculatorMock(grid, errorFunctionalScalingFactor);
    grid2D mockDirectionGrid = directionCalulator->getGrid();

    EXPECT_EQ(grid.getNumberOfGridPoints(), mockDirectionGrid.getNumberOfGridPoints());
    EXPECT_DOUBLE_EQ(grid.getGridStart()[0], mockDirectionGrid.getGridStart()[0]);
    EXPECT_DOUBLE_EQ(grid.getGridStart()[1], mockDirectionGrid.getGridStart()[1]);
    EXPECT_DOUBLE_EQ(grid.getGridEnd()[0], mockDirectionGrid.getGridEnd()[0]);
    EXPECT_DOUBLE_EQ(grid.getGridEnd()[1], mockDirectionGrid.getGridEnd()[1]);
    EXPECT_DOUBLE_EQ(grid.getCellVolume(), mockDirectionGrid.getCellVolume());
    EXPECT_DOUBLE_EQ(grid.getDomainArea(), mockDirectionGrid.getDomainArea());
    EXPECT_DOUBLE_EQ(grid.getCellDimensions()[0], mockDirectionGrid.getCellDimensions()[0]);
    EXPECT_DOUBLE_EQ(grid.getCellDimensions()[1], mockDirectionGrid.getCellDimensions()[1]);
    EXPECT_DOUBLE_EQ(grid.getGridDimensions()[0], mockDirectionGrid.getGridDimensions()[0]);
    EXPECT_DOUBLE_EQ(grid.getGridDimensions()[1], mockDirectionGrid.getGridDimensions()[1]);

    delete directionCalulator;
}

TEST(DirectionCalculatorTest, ConstructorScalingFactorTest)
{
    grid2D grid = getGrid();
    double errorFunctionalScalingFactor = 1.0;

    DirectionCalculator *directionCalulator = new DirectionCalculatorMock(grid, errorFunctionalScalingFactor);
    double mockErrorFunctionalScalingFactor = directionCalulator->getErrorFunctionalScalingFactor();

    EXPECT_EQ(errorFunctionalScalingFactor, mockErrorFunctionalScalingFactor);
}
