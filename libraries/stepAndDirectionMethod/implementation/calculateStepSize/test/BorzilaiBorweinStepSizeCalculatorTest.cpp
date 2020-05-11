#include "BorzilaiBorweinStepSizeCalculator.h"
#include <gtest/gtest.h>

grid2D getGrid()
{
    std::array<double, 2> xMin = {0.0, 0.0};
    std::array<double, 2> xMax = {2.0, 2.0};
    std::array<int, 2> nX = {2, 5};
    grid2D grid(xMin, xMax, nX);

    return grid;
}

TEST(BorzilaiBorweinStepSizeCalculatorTest, calculateStepSizeAndupdateVariablesTest)
{
    grid2D grid = getGrid();
    dataGrid2D chiEstimateCurrent(grid);
    dataGrid2D derivativeCurrent(grid);
    chiEstimateCurrent = 1.0;
    derivativeCurrent = 2.0;

    const double initialStepSize = 0.5;
    //    BorzilaiBorweinStepSizeCalculator stepSizeCalculator(grid);
    StepSizeCalculator *stepSizeCalculator;
    stepSizeCalculator = new BorzilaiBorweinStepSizeCalculator(grid, initialStepSize);

    stepSizeCalculator->updateVariables(chiEstimateCurrent, derivativeCurrent, 1);

    double stepSizeZero = 0.5;
    double stepSizeCalculate = stepSizeCalculator->calculateStepSize();

    ASSERT_DOUBLE_EQ(stepSizeCalculate, stepSizeZero);

    delete stepSizeCalculator;
}
