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
            const std::string *inputPath;
            const core::grid2D *grid;
            const core::Sources *source;
            const core::Receivers *receiver;
            const core::freqInfo *freq;
            const core::FrequenciesGroup *freqGroup;

            virtual void SetUp()
            {
                inputPath = new const std::string(std::string(FWI_PROJECT_DIR) + "/tests");
                grid = new const core::grid2D({0.0, 0.0}, {2.0, 2.0}, {4, 2});
                source = new const core::Sources({0.0, 0.0}, {2.0, 2.0}, 8);
                receiver = new const core::Receivers({0.0, 0.0}, {2.0, 2.0}, 8);
                freq = new const core::freqInfo({10, 40, 15});
                freqGroup = new const core::FrequenciesGroup(*freq, 1.0);
            }

            virtual void TearDown()
            {
                delete inputPath;
                delete grid;
                delete source;
                delete receiver;
                delete freq;
                delete freqGroup;
            }
        };

        TEST_F(IntegralForwardModelTest, CostFunction_InputFromFile_SelectedCostFunction)
        {
            integralForwardModelInputCardReader forwardModelInputReader(*inputPath);
            integralForwardModelInput forwardModelInput(forwardModelInputReader.getInput());

            auto forwardModel = std::make_unique<IntegralForwardModel>(*grid, *source, *receiver, *freqGroup, forwardModelInput);

            EXPECT_EQ(forwardModel->getCostFunction(), forwardModelInput.costFunction);
        }
    }   // namespace forwardModels
}   // namespace fwi
