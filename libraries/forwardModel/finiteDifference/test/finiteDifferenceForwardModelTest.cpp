#include "FiniteDifferenceForwardModel.h"
#include "FiniteDifferenceForwardModelInputCardReader.h"
#include <gtest/gtest.h>

namespace fwi
{
    namespace forwardModels
    {
        class FiniteDifferenceForwardModelTest : public ::testing::Test
        {
        protected:
            const std::string inputPath{std::string(FWI_PROJECT_DIR) + "/tests"};
            const finiteDifferenceForwardModelInputCardReader forwardModelInputReader{inputPath};
            const core::grid2D grid{{0.0, 0.0}, {2.0, 2.0}, {4, 2}};
            const core::Sources source{{0.0, 0.0}, {2.0, 2.0}, 8};
            const core::Receivers receiver{{0.0, 0.0}, {2.0, 2.0}, 8};
            const core::freqInfo freq{10, 40, 15};
            const core::FrequenciesGroup freqGroup{freq, 1.0};

            finiteDifferenceForwardModelInput forwardModelInput;
        };

        TEST_F(FiniteDifferenceForwardModelTest, getCostFunction_InputFromFile_SelectedCostFunction)
        {
            // Arrange
            forwardModelInput = finiteDifferenceForwardModelInput(forwardModelInputReader.getInput());
            finiteDifferenceForwardModel forwardModel(grid, source, receiver, freqGroup, forwardModelInput);

            // Act
            auto costFunction = forwardModel.getCostFunction();

            // Assert
            EXPECT_EQ(costFunction, forwardModelInput.costFunction);
        }

    }   // namespace forwardModels
}   // namespace fwi
