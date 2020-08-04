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
            core::freqInfo freq;

            const double errorFunctionScalingFactor = 1.0;
        };

        TEST_F(ConjugateGradientDirectionCalculatorTest, calculateDirection)
        {
            // Arrange
            core::grid2D grid(xMin, xMax, nX);

            forwardModels::ForwardModelMock *forwardModel = new forwardModels::ForwardModelMock();

            ConjugateGradientDirectionCalculator directionCalculator(errorFunctionScalingFactor, forwardModel);

            core::dataGrid2D chiEstimate(grid);
            std::vector<std::complex<double>> residual(chiEstimate.getNumberOfGridPoints(), 1.0);

            core::dataGrid2D newKappaTimesResidual(grid);
            newKappaTimesResidual = 2.0;

            // Act
            EXPECT_CALL(*forwardModel, getUpdateDirectionInformation(_, _)).WillOnce(testing::SetArgReferee<1>(newKappaTimesResidual));

            std::vector<double> conjugateGradientDirectionData = directionCalculator.calculateDirection(chiEstimate, residual).getData();

            const double expectedDirection = 2.0;

            // Assert
            for(int it = 0; it < chiEstimate.getNumberOfGridPoints(); ++it)
            {
                ASSERT_DOUBLE_EQ(conjugateGradientDirectionData[it], expectedDirection);
            }

            delete forwardModel;
        }
    }   // namespace inversionMethods
}   // namespace fwi
