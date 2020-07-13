#include "integralForwardModel.h"
#include "integralForwardModelInputCardReader.h"
#include <gtest/gtest.h>

namespace fwi
{
    namespace forwardModels
    {
        // Review: consider to use Google test fixtures for this
        const std::string inputPath = std::string(FWI_PROJECT_DIR) + "/tests";

        const core::grid2D grid({0.0, 0.0}, {2.0, 2.0}, {4, 2});
        const core::Sources source({0.0, 0.0}, {2.0, 2.0}, 8);
        const core::Receivers receiver({0.0, 0.0}, {2.0, 2.0}, 8);
        const core::freqInfo freq = {10, 40, 15};
        const core::FrequenciesGroup freqGroup(freq, 1.0);

        // Review: The naming convention we agreed on is a bit different:
        // MethodName_ShortDescriptionInput_ShortDescriptionExpectedOutput
        // ,so I'd expect something like: costFunction_InputFromFile_SelectedCostFunction
        TEST(IntegralForwardModelTest, CostFunctionInInputFileEqualsCostFunctionInForwardModel)
        {
            integralForwardModelInputCardReader forwardModelInputReader(inputPath);
            integralForwardModelInput forwardModelInput(forwardModelInputReader.getInput());

            // Review: Consider using std::make_unique to create a unique pointer
            std::unique_ptr<IntegralForwardModel> forwardModel(new IntegralForwardModel(grid, source, receiver, freqGroup, forwardModelInput));

            EXPECT_EQ(forwardModel->getCostFunction(), forwardModelInput.costFunction);
        }
    }   // namespace forwardModels
}   // namespace fwi
