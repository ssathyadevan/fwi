#include "finiteDifferenceForwardModelInputCardReader.h"
#include "finiteDifferenceForwardModel.h"
#include <gtest/gtest.h>

namespace fwi
{
    namespace forwardModels
    {
        const std::string inputPath = std::string(FWI_PROJECT_DIR) + "/tests";

        TEST(finiteDifferenceForwardModelInputCardReaderTest, ValidInputTest)
        {
            finiteDifferenceForwardModelInputCardReader finiteDifferenceReader(inputPath);
            finiteDifferenceForwardModelInput input = finiteDifferenceReader.getInput();
            EXPECT_EQ(0.0, input.pmlWidthFactor.x);
            EXPECT_EQ(0.0, input.pmlWidthFactor.z);

            EXPECT_EQ(4, input.sourceParameter.r);
            EXPECT_EQ(6.31, input.sourceParameter.beta);

            EXPECT_EQ(leastSquares, input.costFunction);
            EXPECT_EQ(BoundaryConditionType::FirstOrderABC, input.boundaryConditionType);
        }
    }   // namespace forwardModels
}   // namespace fwi
