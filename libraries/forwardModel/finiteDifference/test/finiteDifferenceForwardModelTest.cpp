#include "finiteDifferenceForwardModel.h"
#include "finiteDifferenceForwardModelInputCardReader.h"
#include <gtest/gtest.h>

namespace fwi
{
    namespace forwardModels
    {
        const std::string inputPath = std::string(FWI_PROJECT_DIR) + "/tests";

        const core::grid2D grid({0.0, 0.0}, {2.0, 2.0}, {4, 2});
        const core::Sources source({0.0, 0.0}, {2.0, 2.0}, 8);
        const core::Receivers receiver({0.0, 0.0}, {2.0, 2.0}, 8);
        const core::freqInfo freq = {10, 40, 15};
        const core::FrequenciesGroup freqGroup(freq, 1.0);

        TEST(FiniteDifferenceForwardModelTest, CostFunctionInInputFileEqualsCostFunctionInForwardModel)
        {
            finiteDifferenceForwardModelInputCardReader forwardModelInputReader(inputPath);
            finiteDifferenceForwardModelInput forwardModelInput(forwardModelInputReader.getInput());

            std::unique_ptr<finiteDifferenceForwardModel> forwardModel(new finiteDifferenceForwardModel(grid, source, receiver, freqGroup, forwardModelInput));

            EXPECT_EQ(forwardModel->getCostFunction(), forwardModelInput.costFunction);
        }
    }   // namespace forwardModels
}   // namespace fwi
