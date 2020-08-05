#include "DirectionCalculator.h"
#include "DirectionCalculatorMock.h"
#include "ForwardModelMock.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::_;
using ::testing::NiceMock;
using ::testing::ReturnRef;

namespace fwi
{
    namespace inversionMethods
    {
        class DirectionCalculatorTest : public ::testing::Test
        {
        protected:
            const std::array<double, 2> _xMin{0.0, 0.0};
            const std::array<double, 2> _xMax{2.0, 2.0};
            const std::array<int, 2> _nX{2, 4};
            const core::grid2D _grid{_xMin, _xMax, _nX};
            double _errorFunctionScalingFactor = 1.0;
            NiceMock<forwardModels::ForwardModelMock> _forwardModel;
        };

        TEST_F(DirectionCalculatorTest, ConstructorScalingFactorTest)
        {
            _errorFunctionScalingFactor = 1.0;
            ON_CALL(_forwardModel, getGrid).WillByDefault(ReturnRef(_grid));
            DirectionCalculatorMock directionCalculator(_errorFunctionScalingFactor, &_forwardModel);

            double mockErrorFunctionalScalingFactor = directionCalculator.getErrorFunctionalScalingFactor();

            EXPECT_EQ(_errorFunctionScalingFactor, mockErrorFunctionalScalingFactor);
        }

        TEST_F(DirectionCalculatorTest, ScalingFactorExceptionTest)
        {
            _errorFunctionScalingFactor = -1.0;

            ON_CALL(_forwardModel, getGrid).WillByDefault(testing::ReturnRef(_grid));

            EXPECT_THROW(DirectionCalculatorMock(_errorFunctionScalingFactor, &_forwardModel), std::invalid_argument);
        }
    }   // namespace inversionMethods
}   // namespace fwi
