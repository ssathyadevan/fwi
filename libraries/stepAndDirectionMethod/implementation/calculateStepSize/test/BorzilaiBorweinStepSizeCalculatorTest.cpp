#include "BorzilaiBorweinStepSizeCalculator.h"
#include <gtest/gtest.h>

TEST(BorzilaiBorweinStepSizeCalculatorTest, calculateStepSizeTest)
{
    // I have to work more on this, and also use calculateStepSize to test updateVariables, since it's the only way to reach the Previous ones

    std::array<double, 2> xMin = {0.0, 0.0};
    std::array<double, 2> xMax = {2.0, 2.0};
    std::array<int, 2> nX = {2, 5};

    grid2D grid(xMin, xMax, nX);
    // initializing dataGrid2D from grid2D initializes _data[] to zeroes
    dataGrid2D dataGridZero(grid);
    dataGrid2D dataGridTwo(grid);

    for(int i = 0; i < dataGridTwo.getNumberOfGridPoints(); ++i)
    {
        //      dataGridZero.setValueAtIndex(0.0, i);
        dataGridTwo.setValueAtIndex(2.0, i);
    }

    BorzilaiBorweinStepSizeCalculator stepSizeCalculator(grid);
    // if I keep derivativeCurrent=derivativePrevious I end up dividing by zero, so I need to update one of them

    stepSizeCalculator.setCurrentVariables(dataGridZero, dataGridTwo);

    double stepSizeZero = 0.0;
    double stepSizeCalculate = stepSizeCalculator.calculateStepSize();
    std::cout << "stepSizeCalculate = " << stepSizeCalculate << std::endl;

    ASSERT_DOUBLE_EQ(stepSizeCalculate, stepSizeZero);
}

TEST(BorzilaiBorweinStepSizeCalculatorTest, setCurrentVariablesTest)
{
    std::array<double, 2> xMin = {0.0, 0.0};
    std::array<double, 2> xMax = {2.0, 2.0};
    std::array<int, 2> nX = {2, 5};

    grid2D grid(xMin, xMax, nX);

    dataGrid2D dataGridOne(grid);
    dataGrid2D dataGridTwo(grid);

    for(int i = 0; i < dataGridTwo.getNumberOfGridPoints(); ++i)
    {
        dataGridOne.setValueAtIndex(1.0, i);
        dataGridTwo.setValueAtIndex(2.0, i);
    }

    BorzilaiBorweinStepSizeCalculator stepSizeCalculator(grid);

    stepSizeCalculator.setCurrentVariables(dataGridOne, dataGridTwo);

    bool chiEstimateCurrentEqual = (stepSizeCalculator.getChiEstimateCurrent().getData() == dataGridOne.getData());

    bool derivativeCurrentEqual = (stepSizeCalculator.getDerivativeCurrent().getData() == dataGridTwo.getData());

    ASSERT_EQ(chiEstimateCurrentEqual, true);
    ASSERT_EQ(derivativeCurrentEqual, true);
}

TEST(BorzilaiBorweinStepSizeCalculatorTest, updateVariablesTest)
{
    std::array<double, 2> xMin = {0.0, 0.0};
    std::array<double, 2> xMax = {2.0, 2.0};

    std::array<int, 2> nX = {2, 4};

    grid2D grid(xMin, xMax, nX);

    BorzilaiBorweinStepSizeCalculator stepSizeCalculator(grid);

    dataGrid2D dataGridOne(grid);
    dataGrid2D dataGridTwo(grid);

    for(int i = 0; i < dataGridTwo.getNumberOfGridPoints(); ++i)
    {
        dataGridOne.setValueAtIndex(1.0, i);
        dataGridTwo.setValueAtIndex(2.0, i);
    }
    stepSizeCalculator.setCurrentVariables(dataGridOne, dataGridOne);
    stepSizeCalculator.updateVariables(dataGridTwo, dataGridTwo);

    double stepSizeCalculate = stepSizeCalculator.calculateStepSize();

    bool updateVariablesTrue = stepSizeCalculate == 1.0;

    ASSERT_EQ(updateVariablesTrue, true);
}
