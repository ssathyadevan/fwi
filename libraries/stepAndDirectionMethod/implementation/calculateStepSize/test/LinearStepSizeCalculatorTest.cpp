#include "LinearStepSizeCalculator.h"
#include <gtest/gtest.h>

grid2D getGrid()
{
    std::array<double, 2> xMin = {0.0, 0.0};
    std::array<double, 2> xMax = {2.0, 2.0};
    std::array<int, 2> nX = {2, 5};
    grid2D grid(xMin, xMax, nX);

    return grid;
}

TEST(LinearStepSizeCalculatorTest, constructorTest)
{
    double validParameter = 2.0;
    double invalidParameter = 0.0;

    StepSizeCalculator *stepSizeCalculator;
    EXPECT_THROW(stepSizeCalculator = new LinearStepSizeCalculator(validParameter, invalidParameter), std::invalid_argument);

    EXPECT_THROW(stepSizeCalculator = new LinearStepSizeCalculator(invalidParameter, validParameter), std::invalid_argument);

    EXPECT_THROW(stepSizeCalculator = new LinearStepSizeCalculator(invalidParameter, invalidParameter), std::invalid_argument);

    stepSizeCalculator = new LinearStepSizeCalculator(validParameter, validParameter);

    EXPECT_DOUBLE_EQ(stepSizeCalculator->calculateStepSize(), 2.0);

    delete stepSizeCalculator;
}

TEST(LinearStepSizeCalculatorTest, calculateStepSizeAndupdateVariablesTest)
{
    grid2D grid = getGrid();
    dataGrid2D dataGrid(grid);

    LinearStepSizeCalculator stepSizeCalculator(1.0, 1.0);

    stepSizeCalculator.updateVariables(dataGrid, dataGrid, 1);

    EXPECT_DOUBLE_EQ(stepSizeCalculator.calculateStepSize(), 0.5);
}