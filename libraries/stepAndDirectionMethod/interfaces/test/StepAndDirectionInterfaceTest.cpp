#include "StepAndDirectionInterface.h"
#include "ConjugateGradientDirectionCalculator.h"
#include "FixedStepSizeCalculator.h"
#include "forwardmodelinterfacemock.h"
#include <gtest/gtest.h>

TEST(StepAndDirectionInterfaceTest, ConstructorStepAndDirectionInterfaceTest)
{
    double eta = 1.0;
    std::array<double, 2> xMin = {0, 1.0};
    std::array<double, 2> xMax = {0, 1.0};
    std::array<int, 2> nX = {1, 1};
    int nSrc = 2;
    int nRecv = 2;
    freqInfo freqStruct;   // default constructor
    double c0 = 1.0;
    grid2D grid(xMin, xMax, nX);
    dataGrid2D dataGrid(grid);
    sources src(xMin, xMax, nSrc);
    receivers recv(xMin, xMax, nRecv);
    frequenciesGroup freq(freqStruct, c0);

    StepSizeCalculator *chosenStepSizeTest = new FixedStepSizeCalculator(2.0);
    DirectionInput directionInputTest(dataGrid);
    forwardModelInterface *forwardModelTest = new ForwardModelInterfaceMock(grid, src, recv, freq);

    DirectionCalculator *chosenDirectionTest = new ConjugateGradientDirectionCalculator(eta, forwardModelTest);
    //    StepAndDirectionInterface SADITest(*chosenStepSizeTest, *chosenDirectionTest, *forwardModelTest, directionInputTest);

    //    EXPECT_EQ(SADITest.getChosenStep(), chosenStepTest);

    delete chosenStepSizeTest;
    delete chosenDirectionTest;
    delete forwardModelTest;
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
