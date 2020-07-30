#include "CostFunctionCalculator.h"
#include <gtest/gtest.h>
#include <memory.h>

namespace fwi
{
    namespace core
    {
        class CostFunctionCalculatorTest : public ::testing::Test
        {
        protected:
            const CostFunctionCalculator costCalculator;
        };

        TEST_F(CostFunctionCalculatorTest, getCostFunction_CostFunctionEnumValidValue_SelectedCostFunction)
        {
            // Arrange
            CostFunctionCalculator costCalculatora(CostFunctionCalculator::CostFunctionEnum::leastSquares);
            // Act

            // Assert
            EXPECT_EQ(costCalculatora.getCostFunction(), CostFunctionCalculator::CostFunctionEnum::leastSquares);
        }
    }   // namespace core
}   // namespace fwi
