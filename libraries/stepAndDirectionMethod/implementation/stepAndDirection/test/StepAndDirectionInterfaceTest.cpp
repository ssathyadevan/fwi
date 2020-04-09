#include "StepAndDirectionInterface.h"
#include <gtest/gtest.h>

// grid2D getGrid()
//{
//    std::array<double, 2> xMin = {0.0, 0.0};
//    std::array<double, 2> xMax = {2.0, 2.0};
//    std::array<int, 2> nX = {2, 4};

//    grid2D grid(xMin, xMax, nX);
//    return grid;
//}

class dataGrid2D;

class chosenStep
{
    double calculateStep(dataGrid2D chiEstimatePrevious, dataGrid2D chiEstimateCurrent, dataGrid2D directionPrevious, dataGrid2D directionCurrent)
    {
        chiEstimatePrevious.zero();
        chiEstimateCurrent.zero();
        directionPrevious.zero();
        directionCurrent.zero();
        return 1.0;
    }
};

TEST(StepAndDirectionInterfaceTest, ConstructorStepAndDirectionInterfaceTest)
{
    //    grid2D grid = getGrid();
    //    double eta = 1.0;

    //    DirectionCalculator *directionCalulator = new ConjugateGradientDirectionCalculator(grid, eta);
    //    grid2D cGDirectionGrid = directionCalulator->getGrid();

    //    EXPECT_EQ(grid.getNumberOfGridPoints(), cGDirectionGrid.getNumberOfGridPoints());
    //    EXPECT_DOUBLE_EQ(grid.getGridStart()[0], cGDirectionGrid.getGridStart()[0]);
    //    EXPECT_DOUBLE_EQ(grid.getGridStart()[1], cGDirectionGrid.getGridStart()[1]);
    //    EXPECT_DOUBLE_EQ(grid.getGridEnd()[0], cGDirectionGrid.getGridEnd()[0]);
    //    EXPECT_DOUBLE_EQ(grid.getGridEnd()[1], cGDirectionGrid.getGridEnd()[1]);
    //    EXPECT_DOUBLE_EQ(grid.getCellVolume(), cGDirectionGrid.getCellVolume());
    //    EXPECT_DOUBLE_EQ(grid.getDomainArea(), cGDirectionGrid.getDomainArea());
    //    EXPECT_DOUBLE_EQ(grid.getCellDimensions()[0], cGDirectionGrid.getCellDimensions()[0]);
    //    EXPECT_DOUBLE_EQ(grid.getCellDimensions()[1], cGDirectionGrid.getCellDimensions()[1]);
    //    EXPECT_DOUBLE_EQ(grid.getGridDimensions()[0], cGDirectionGrid.getGridDimensions()[0]);
    //    EXPECT_DOUBLE_EQ(grid.getGridDimensions()[1], cGDirectionGrid.getGridDimensions()[1]);

    //    delete directionCalulator;
}

// TEST(conjugateGradientDirectionCalculatorTest, ConstructorDirectionTest)
//{
//    grid2D grid = getGrid();
//    double eta = 1.0;

//    DirectionCalculator *directionCalulator = new ConjugateGradientDirectionCalculator(grid, eta);
//    dataGrid2D initializedDirection = directionCalulator->getDirection();

//    const int nrOfGridPoints = initializedDirection.getNumberOfGridPoints();
//    const std::vector<double> &data = initializedDirection.getData();
//    for(int i = 0; i < nrOfGridPoints; i++)
//    {
//        ASSERT_DOUBLE_EQ(data[i], 0.0);
//    }

//    delete directionCalulator;
//}

// TEST(conjugateGradientDirectionCalculatorTest, calculateDirectionTest)
//{
//    grid2D grid = getGrid();
//    double eta = 1.0;
//    complexDataGrid2D residuals(grid);

//    DirectionCalculator *directionCalulator = new ConjugateGradientDirectionCalculator(grid, eta);
//    dataGrid2D cGDirection(grid);
//    directionCalulator->calculateDirection(residuals);
//    cGDirection = directionCalulator->getDirection();

//    const int nrOfGridPoints = cGDirection.getNumberOfGridPoints();
//    const std::vector<double> &data = cGDirection.getData();
//    for(int i = 0; i < nrOfGridPoints; i++)
//    {
//        ASSERT_DOUBLE_EQ(data[i], 1.0);
//    }

//    delete directionCalulator;
//}

// TEST(conjugateGradientDirectionCalculatorTest, calculateDirectionExceptionTest)
//{
//    grid2D grid = getGrid();
//    double eta = 1.0;
//    dataGrid2D data1(grid);
//    dataGrid2D data2(grid);

//    DirectionCalculator *directionCalulator = new ConjugateGradientDirectionCalculator(grid, eta);
//    dataGrid2D cGDirection(grid);
//    EXPECT_THROW(directionCalulator->calculateDirection(data1, data2), std::bad_exception);
//}
