#include "ConjugateGradientDirectionCalculator.h"
#include "ForwardModelMock.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::_;

namespace fwi
{
    namespace inversionMethods
    {
        class ConjugateGradientDirectionCalculatorTest : public ::testing::Test
        {
        protected:
            std::array<double, 2> xMin = {0.0, 0.0};
            std::array<double, 2> xMax = {2.0, 2.0};
            std::array<int, 2> nX = {2, 4};
            const double errorFunctionScalingFactor = 1.0;
        };

        TEST_F(ConjugateGradientDirectionCalculatorTest, calculateDirection_OneIteration_ExpectVectorOfFour)
        {
            // Arrange
            core::grid2D grid(xMin, xMax, nX);
            testing::NiceMock<forwardModels::ForwardModelMock> forwardModel;
            ON_CALL(forwardModel, getGrid).WillByDefault(testing::ReturnRef(grid));
            ConjugateGradientDirectionCalculator directionCalculator(errorFunctionScalingFactor, &forwardModel);
            core::dataGrid2D chiEstimate(grid);
            std::vector<std::complex<double>> residual(chiEstimate.getNumberOfGridPoints(), 1.0);

            // Act
            // assigning kappaTimesResidual = 2.0
            EXPECT_CALL(forwardModel, getUpdateDirectionInformation(_, _)).WillOnce(testing::SetArgReferee<1>(2.0));
            std::vector<double> conjugateGradientDirectionData = directionCalculator.calculateDirection(chiEstimate, residual).getData();

            const double expectedDirection = 4.0;

            // Assert
            for(int it = 0; it < chiEstimate.getNumberOfGridPoints(); ++it)
            {
                ASSERT_DOUBLE_EQ(conjugateGradientDirectionData[it], expectedDirection);
            }
        }

        TEST_F(ConjugateGradientDirectionCalculatorTest, calculateDirection_TwoIterations_ExpectVectorOfPointFive)
        {
            // Arrange
            core::grid2D grid(xMin, xMax, nX);
            testing::NiceMock<forwardModels::ForwardModelMock> forwardModel;
            ON_CALL(forwardModel, getGrid).WillByDefault(testing::ReturnRef(grid));
            ConjugateGradientDirectionCalculator directionCalculator(errorFunctionScalingFactor, &forwardModel);
            core::dataGrid2D chiEstimate(grid);
            std::vector<std::complex<double>> residual(chiEstimate.getNumberOfGridPoints(), 1.0);

            // Act
            // assigning kappaTimesResidual = 1.0
            EXPECT_CALL(forwardModel, getUpdateDirectionInformation(_, _)).WillOnce(testing::SetArgReferee<1>(1.0));
            directionCalculator.calculateDirection(chiEstimate, residual).getData();

            // assigning kappaTimesResidual = 0.5
            EXPECT_CALL(forwardModel, getUpdateDirectionInformation(_, _)).WillOnce(testing::SetArgReferee<1>(0.5));
            std::vector<double> conjugateGradientDirectionData = directionCalculator.calculateDirection(chiEstimate, residual).getData();

            const double expectedDirection = 0.5;

            // Assert
            for(int it = 0; it < chiEstimate.getNumberOfGridPoints(); ++it)
            {
                ASSERT_DOUBLE_EQ(conjugateGradientDirectionData[it], expectedDirection);
            }
        }
    }   // namespace inversionMethods
}   // namespace fwi
