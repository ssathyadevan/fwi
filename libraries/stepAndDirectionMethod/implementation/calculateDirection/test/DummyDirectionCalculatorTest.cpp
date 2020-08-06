#include "DummyDirectionCalculator.h"
#include "ForwardModelMock.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnRef;

namespace fwi
{
    namespace inversionMethods
    {
        /**
         * @brief This test can be used as a base to create the unit test for any customized DirectionCalculator
         */

        class DummyDirectionCalculatorTest : public ::testing::Test
        {
        public:
            const std::array<double, 2> _xMin{0.0, 0.0};
            const std::array<double, 2> _xMax{2.0, 2.0};
            const std::array<int, 2> _nX{2, 4};
            const core::freqInfo _freq{0.0, 10.0, 5};
            const core::grid2D _grid{_xMin, _xMax, _nX};
            const core::Sources _sources{_xMin, _xMax, 2};
            const core::Receivers _receivers{_xMin, _xMax, 2};
            const core::FrequenciesGroup _frequencies{_freq, 2000.0};
            const double _errorFunctionalScalingFactor = 1.0;
            NiceMock<forwardModels::ForwardModelMock> _forwardModel;
        };

        TEST_F(DummyDirectionCalculatorTest, CalculateDirectionTest_ResidualVectorIsZero_ResultingDirectionIsZero)
        {
            // Arrange
            ON_CALL(_forwardModel, getGrid).WillByDefault(ReturnRef(_grid));
            DummyDirectionCalculator directionCalculator(_errorFunctionalScalingFactor, &_forwardModel);
            const core::dataGrid2D chiEstimate(_grid);
            const std::vector<std::complex<double>> residuals(_grid.getNumberOfGridPoints(), 0.0);

            // Fill the expectedDirection vector with the output you expect from the directionCalculator->calculateDirection() method
            const std::vector<double> expectedDirection(_grid.getNumberOfGridPoints(), 0.0);

            // Act
            std::vector<double> const dummyDirection = directionCalculator.calculateDirection(chiEstimate, residuals).getData();

            // Assert
            for(size_t it = 0; it < dummyDirection.size(); ++it)
            {
                // if the values differ by small truncament errors, consider EXPECT_NEAR(value1, value2, tolerance);
                ASSERT_DOUBLE_EQ(dummyDirection[it], expectedDirection[it]);
            }
        }
    }   // namespace inversionMethods
}   // namespace fwi
