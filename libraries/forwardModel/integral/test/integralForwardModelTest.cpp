#include "integralForwardModel.h"
#include "integralForwardModelInputCardReader.h"
#include <gtest/gtest.h>

namespace fwi
{
    namespace forwardModels
    {
        class IntegralForwardModelTest : public ::testing::Test
        {
        protected:
        protected:
            const std::string inputPath{std::string(FWI_PROJECT_DIR) + "/tests"};
            const integralForwardModelInputCardReader forwardModelInputReader{inputPath};
            const core::grid2D grid{{0.0, 0.0}, {2.0, 2.0}, {4, 2}};
            const core::Sources source{{0.0, 0.0}, {2.0, 2.0}, 8};
            const core::Receivers receiver{{0.0, 0.0}, {2.0, 2.0}, 8};
            const core::freqInfo freq{10, 40, 15};
            const core::FrequenciesGroup freqGroup{freq, 1.0};

            integralForwardModelInput forwardModelInput;
        };

        TEST_F(IntegralForwardModelTest, getCostFunction_InputFromFile_SelectedCostFunction)
        {
            // Arrange
            forwardModelInput = integralForwardModelInput(forwardModelInputReader.getInput());
            IntegralForwardModel forwardModel(grid, source, receiver, freqGroup, forwardModelInput);

            // Act
            auto costFunction = forwardModel.getCostFunction();

            // Assert
            EXPECT_EQ(costFunction, forwardModelInput.costFunction);
        }
    }   // namespace forwardModels
}   // namespace fwi
