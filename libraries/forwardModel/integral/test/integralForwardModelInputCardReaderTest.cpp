#include "integralForwardModelInputCardReader.h"
#include "integralForwardModel.h"
#include <gtest/gtest.h>

namespace forwardModels
{
    // const std::string inputPath = "./../../../parallelized-fwi/tests";   // For local use
    const std::string inputPath = "./../../../../../tests";

    TEST(integralForwardModelInputCardReaderTest, ValidInputTest)
    {
        integralForwardModelInputCardReader integralForwardModelReader(inputPath);
        integralForwardModelInput input = integralForwardModelReader.getInput();

        EXPECT_EQ(15, input.nrOfIterations);
        EXPECT_EQ(5.0e-5, input.tolerance);
        ASSERT_FALSE(input.calcAlpha);
    }
}   // namespace forwardModels
