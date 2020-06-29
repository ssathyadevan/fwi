#include "DummyStepSizeCalculator.h"
#include <gtest/gtest.h>

TEST(DummyStepSizeCalculator, calculateStepSize_noInput_doubleFourPointTwo)
{
    double dummyStepSize = 4.2;
    StepSizeCalculator *stepSizeCalculator = new DummyStepSizeCalculator(dummyStepSize);

    ASSERT_DOUBLE_EQ(stepSizeCalculator->calculateStepSize(), dummyStepSize);
    delete stepSizeCalculator;
}
