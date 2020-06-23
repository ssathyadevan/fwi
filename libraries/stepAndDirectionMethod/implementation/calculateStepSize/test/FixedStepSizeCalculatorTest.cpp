#include "FixedStepSizeCalculator.h"
#include <gtest/gtest.h>

namespace inversionMethods
{
    core::grid2D getGrid()
    {
        std::array<double, 2> xMin = {0.0, 0.0};
        std::array<double, 2> xMax = {2.0, 2.0};
        std::array<int, 2> nX = {2, 4};

        core::grid2D grid(xMin, xMax, nX);
        return grid;
    }

    TEST(fixedStepSizeCalculatorTest, calculateStepSizeTest)
    {
        double stepSizeInput = 1.1;
        StepSizeCalculator *stepSizeCalculator = new FixedStepSizeCalculator(stepSizeInput);

        double stepSizeCalculate;
        stepSizeCalculate = stepSizeCalculator->calculateStepSize();

        ASSERT_DOUBLE_EQ(stepSizeCalculate, stepSizeInput);
        delete stepSizeCalculator;
    }
}   // namespace inversionMethods
