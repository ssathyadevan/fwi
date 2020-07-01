#include "gradientDescentInversionInputCardReader.h"
#include <gtest/gtest.h>

namespace fwi
{
    namespace inversionMethods
    {
        const std::string inputPath = std::string(FWI_PROJECT_DIR) + "/tests";

        TEST(GradientDescentInversionInputCardReaderTest, constructor_ValidInput)
        {
            gradientDescentInversionInputCardReader gradientDescentReader(inputPath);
            gradientDescentInversionInput input = gradientDescentReader.getInput();
            ASSERT_DOUBLE_EQ(0.1, input.gamma0);
            ASSERT_DOUBLE_EQ(0.001, input.x0);
            ASSERT_DOUBLE_EQ(0.001, input.h);
            EXPECT_EQ(20, input.iter);
        }

        const std::string invalidInputPath = std::string(FWI_PROJECT_DIR) + "/tests/invalidInput";
        TEST(GradientDescentInversionInputCardReaderTest, construtor_InvalidInput_ExceptionsThrown)
        {   // Only tests that the variable 'h' cannot be <=0
            EXPECT_THROW(gradientDescentInversionInputCardReader gradientDescentReader(invalidInputPath), std::invalid_argument);
        }
    }   // namespace inversionMethods
}   // namespace fwi
