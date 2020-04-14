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
    StepAndDirectionInterface SADITest(chosenStepSizeTest, chosenDirectionTest, forwardModelTest, directionInputTest);

    // how to check the constructor??
    // EXPECT_EQ(SADITest.getChosenStep(), chosenStepTest);

    delete chosenStepSizeTest;
    delete chosenDirectionTest;
    delete forwardModelTest;

    //    EXPECT_DOUBLE_EQ(grid.getGridStart()[0], cGDirectionGrid.getGridStart()[0]);

    //    delete directionCalulator;
}

TEST(StepAndDirectionInterfaceTest, reconstructTest)
{
    // how to I test this one?
}

TEST(StepAndDirectionInterfaceTest, calculateNextMoveInterfaceTest)
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
    StepAndDirectionInterface SADITest(chosenStepSizeTest, chosenDirectionTest, forwardModelTest, directionInputTest);
    dataGrid2D directionTest(grid);
    directionTest.zero();
    directionTest = directionTest + 1.0;
    dataGrid2D chiEstimateTest(grid);
    chiEstimateTest.zero();
    double step = 2.0;
    dataGrid2D nextMoveTest(grid);
    nextMoveTest.zero();

    nextMoveTest = SADITest.calculateNextMove(chiEstimateTest, directionTest, step);

    for(unsigned int i = 0; i < nextMoveTest.getData().size(); ++i)
    {
        std::cout << "Element number " << i << " of nextMoveTest = " << nextMoveTest.getData()[i] << std::endl;
    }
}

TEST(StepAndDirectionInterfaceTest, functionFTest) {}

TEST(StepAndDirectionInterfaceTest, normSqTest) {}
