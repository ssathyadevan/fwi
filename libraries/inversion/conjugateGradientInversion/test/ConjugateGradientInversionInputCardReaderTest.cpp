#include "conjugateGradientInversionInputCardReader.h"
#include <gtest/gtest.h>

namespace fwi
{
    namespace inversionMethods
    {
        const std::string inputPath = std::string(FWI_PROJECT_DIR) + "/tests";

        TEST(ConjugateGradientInversionInputCardReaderTest, constructor_ValidInput)
        {
            ConjugateGradientInversionInputCardReader conjugateGradientReader(inputPath);
            ConjugateGradientInversionInput input = conjugateGradientReader.getInput();
            EXPECT_EQ(10, input.iteration1.n);
            ASSERT_DOUBLE_EQ(1e-8, input.iteration1.tolerance);

            ASSERT_DOUBLE_EQ(100, input.dAmplification.start);
            ASSERT_DOUBLE_EQ(10, input.dAmplification.slope);

            EXPECT_EQ(5, input.n_max);
            EXPECT_EQ(true, input.doRegularisation);
        }

        const std::string invalidInputPath = std::string(FWI_PROJECT_DIR) + "/tests/invalidInput";
        TEST(ConjugateGradientInversionInputCardReaderTest, construtor_InvalidInput_ExceptionsThrown)
        {   // Only tests that the variable "n_max" cannot be <=0
            EXPECT_THROW(ConjugateGradientInversionInputCardReader conjugateGradientReader(invalidInputPath), std::invalid_argument);
        }
    }   // namespace inversionMethods
}   // namespace fwi
