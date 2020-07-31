#include "CostFunctionCalculator.h"
#include <gtest/gtest.h>
#include <memory.h>

namespace fwi
{
    namespace core
    {
        TEST(CostFunctionCalculatorTest, getCostFunction_CostFunctionEnumValidValue_SelectedCostFunction)
        {
            // Arrange
            CostFunctionCalculator costCalculator(CostFunctionCalculator::CostFunctionEnum::leastSquares);
            // Act

            // Assert
            EXPECT_EQ(costCalculator.getCostFunction(), CostFunctionCalculator::CostFunctionEnum::leastSquares);
        }
    }   // namespace core
}   // namespace fwi
