#include "ConjugateGradientDirectionCalculator.h"
#include "FixedStepSizeCalculator.h"
#include "StepAndDirectionInterface.h"
#include "forwardmodelinterfacemock.h"
#include <gtest/gtest.h>

TEST(StepAndDirectionImplementationTest, ConstructorStepAndDirectionImplementationTest)
{
    double eta = 1.0;
    std::array<double, 2> xMin = {0, 1.0};
    std::array<double, 2> xMax = {0, 1.0};
    std::array<int, 2> nX = {1, 1};
    int nSrc = 2;
    int nRecv = 2;
    freqInfo freqStruct;
    double c0 = 1.0;
    grid2D gridTest(xMin, xMax, nX);
    dataGrid2D dataGrid(gridTest);
    sources src(xMin, xMax, nSrc);
    receivers recv(xMin, xMax, nRecv);
    frequenciesGroup freq(freqStruct, c0);

    FixedStepSizeCalculator chosenStepSizeTest(2.0);
    ForwardModelInterfaceMock forwardModelTest(gridTest, src, recv, freq);
    ConjugateGradientDirectionCalculator chosenDirectionTest(eta, &forwardModelTest);
    DirectionInput directionInputTest(dataGrid);

    StepAndDirectionInterface SADITest(&chosenStepSizeTest, &chosenDirectionTest, &forwardModelTest, directionInputTest);

    StepSizeCalculator *stepSizeCheck = SADITest.getChosenStep();
    bool stepSizeIsNotDerived = stepSizeCheck == NULL;
    EXPECT_FALSE(stepSizeIsNotDerived);

    DirectionCalculator *directionCheck = SADITest.getChosenDirection();
    bool directionIsNotDerived = directionCheck == NULL;
    EXPECT_FALSE(directionIsNotDerived);

    forwardModelInterface *fMICheck = SADITest.getForwardModel();
    bool fMIIsNotDerived = fMICheck == NULL;
    EXPECT_FALSE(fMIIsNotDerived);

    EXPECT_EQ(SADITest.getDirectionInput()._startingChi.getGrid(), gridTest);
}

// TEST(StepAndDirectionImplementationTest, reconstructTest)
//{
//    // how to I test this one? It's too big and generic
//}

TEST(StepAndDirectionImplementationTest, calculateNextMoveInterfaceTest)
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

    FixedStepSizeCalculator chosenStepSizeTest(2.0);
    ForwardModelInterfaceMock forwardModelTest(grid, src, recv, freq);
    ConjugateGradientDirectionCalculator chosenDirectionTest(eta, &forwardModelTest);
    DirectionInput directionInputTest(dataGrid);

    StepAndDirectionInterface SADITest(&chosenStepSizeTest, &chosenDirectionTest, &forwardModelTest, directionInputTest);
    dataGrid2D directionTest(grid);
    directionTest.zero();
    directionTest = directionTest + 1.0;
    dataGrid2D chiEstimateTest(grid);
    chiEstimateTest.zero();
    double step = 2.0;
    dataGrid2D nextMoveTest(grid);
    nextMoveTest.zero();

    nextMoveTest = SADITest.calculateNextMove(chiEstimateTest, directionTest, step);
    std::vector<double> nextMoveTestData = nextMoveTest.getData();

    for(int i = 0; i < (int)nextMoveTestData.size(); ++i)
    {
        EXPECT_DOUBLE_EQ(nextMoveTestData[i], chiEstimateTest.getData()[i] - step * directionTest.getData()[i]);
    }
}

// TEST(StepAndDirectionImplementationTest, functionFTest)
//{
//    // this function simply invokes two functions from forwardModel
//}

TEST(StepAndDirectionImplementationTest, normSqTest)
{
    double eta = 1.0;
    std::array<double, 2> xMin = {0, 1.0};
    std::array<double, 2> xMax = {0, 1.0};
    std::array<int, 2> nX = {1, 1};
    int nSrc = 2;
    int nRecv = 2;
    freqInfo freqStruct;
    double c0 = 1.0;
    grid2D gridTest(xMin, xMax, nX);
    dataGrid2D dataGrid(gridTest);
    sources src(xMin, xMax, nSrc);
    receivers recv(xMin, xMax, nRecv);
    frequenciesGroup freq(freqStruct, c0);

    FixedStepSizeCalculator chosenStepSizeTest(2.0);
    ForwardModelInterfaceMock forwardModelTest(gridTest, src, recv, freq);
    ConjugateGradientDirectionCalculator chosenDirectionTest(eta, &forwardModelTest);
    DirectionInput directionInputTest(dataGrid);

    StepAndDirectionInterface SADITest(&chosenStepSizeTest, &chosenDirectionTest, &forwardModelTest, directionInputTest);

    std::vector<std::complex<double>> pDataTestNull = {{0, 0}, {0, 0}, {0, 0}};
    std::vector<std::complex<double>> pDataTestReal = {{1, 0}, {1, 0}, {1, 0}};
    std::vector<std::complex<double>> pDataTestImg = {{0, 1}, {0, 1}, {0, 1}};
    EXPECT_DOUBLE_EQ(SADITest.normSq(pDataTestNull), 0.0);
    EXPECT_DOUBLE_EQ(SADITest.normSq(pDataTestReal), 3.0);
    EXPECT_DOUBLE_EQ(SADITest.normSq(pDataTestImg), 3.0);
}
