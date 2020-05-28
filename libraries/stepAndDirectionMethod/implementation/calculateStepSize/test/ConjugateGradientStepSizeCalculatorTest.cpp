#include "ConjugateGradientStepSizeCalculator.h"
#include <gtest/gtest.h>

grid2D getGrid()
{
    std::array<double, 2> xMin = {0.0, 0.0};
    std::array<double, 2> xMax = {2.0, 2.0};
    std::array<int, 2> nX = {2, 5};
    grid2D grid(xMin, xMax, nX);

    return grid;
}

TEST(ConjugateGradientStepSizeCalculatorTest, calculateStepSizeAndUpdateVariables)
{
    grid2D grid = getGrid();
    double nGridPoints = grid.getNumberOfGridPoints();
    dataGrid2D chiEstimateCurrent(grid);
    dataGrid2D derivativeCurrent(grid);

    const double initialStepSize = 0.5;

    std::vector<std::complex<double>> kappaTimesZeta(nGridPoints);
    std::vector<std::complex<double>> residualVector(nGridPoints);

    StepSizeCalculator *stepSizeCalculator = new ConjugateGradientStepSizeCalculator(grid, initialStepSize);
    double stepSizeTest = 0.0;

    // if kappaTimesZeta =0
    EXPECT_THROW(stepSizeTest = stepSizeCalculator->calculateStepSize(), std::overflow_error);

    // updating kappaTimesZeta and residualVector
    std::complex<double> tmpComplexOne(1, 0);
    std::complex<double> tmpComplexTwo(2, 0);

    for(int i = 0; i < nGridPoints; ++i)
    {
        kappaTimesZeta[i] += tmpComplexOne;
        residualVector[i] += tmpComplexTwo;
    }

    stepSizeCalculator->updateVariables(chiEstimateCurrent, derivativeCurrent, 1, kappaTimesZeta, residualVector);

    double stepSizeBenchmark = 2.0;
    stepSizeTest = stepSizeCalculator->calculateStepSize();
    ASSERT_DOUBLE_EQ(stepSizeTest, stepSizeBenchmark);

    delete stepSizeCalculator;
}
