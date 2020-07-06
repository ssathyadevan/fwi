#include "DummyStepSizeCalculator.h"
#include <gtest/gtest.h>

namespace fwi
{
    namespace inversionMethods
    {
        TEST(DummyStepSizeCalculator, calculateStepSize_noInput_doubleFourPointTwo)
        {
            double dummyStepSize = 4.2;
            StepSizeCalculator *stepSizeCalculator = new DummyStepSizeCalculator(dummyStepSize);

            ASSERT_DOUBLE_EQ(stepSizeCalculator->calculateStepSize(), dummyStepSize);
            delete stepSizeCalculator;
        }
    }   // namespace inversionMethods
}   // namespace fwi
