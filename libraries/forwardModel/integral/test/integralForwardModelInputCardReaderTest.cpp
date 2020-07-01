#include "integralForwardModelInputCardReader.h"
#include "integralForwardModel.h"
#include <gtest/gtest.h>

namespace fwi
{
    namespace forwardModels
    {
        const std::string validInputPath = std::string(FWI_PROJECT_DIR) + "/tests";
        TEST(integralForwardModelInputCardReaderTest, constructor_ValidInput)
        {
            integralForwardModelInputCardReader integralForwardModelReader(validInputPath);
            integralForwardModelInput input = integralForwardModelReader.getInput();

            EXPECT_EQ(15, input.nrOfIterations);
            EXPECT_EQ(5.0e-5, input.tolerance);
            ASSERT_FALSE(input.calcAlpha);
            EXPECT_EQ(leastSquares, input.costFunction);
        }

        const std::string invalidInputPath = std::string(FWI_PROJECT_DIR) + "/tests/invalidInput";
        TEST(integralForwardModelInputCardReaderTest, constructor_InvalidInput_ExceptionsThrown)
        {   // Only the first invalid input is checked out of two
            EXPECT_THROW(integralForwardModelInputCardReader integralForwardModelReader(invalidInputPath), std::invalid_argument);
        }
    }   // namespace forwardModels
}   // namespace fwi
