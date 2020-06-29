#include "integralForwardModelInputCardReader.h"
#include "integralForwardModel.h"
#include <gtest/gtest.h>

namespace fwi
{
    namespace forwardModels
    {
        const std::string inputPath = std::string(FWI_PROJECT_DIR) + "/tests";

        TEST(integralForwardModelInputCardReaderTest, ValidInputTest)
        {
            integralForwardModelInputCardReader integralForwardModelReader(inputPath);
            integralForwardModelInput input = integralForwardModelReader.getInput();

            EXPECT_EQ(15, input.nrOfIterations);
            EXPECT_EQ(5.0e-5, input.tolerance);
            ASSERT_FALSE(input.calcAlpha);
        }
    }   // namespace forwardModels
}   // namespace fwi
