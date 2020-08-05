#include "GradientDescentDirectionCalculator.h"
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
        class GradientDescentDirectionCalculatorTest : public ::testing::Test
        {
        public:
            const core::CostFunctionCalculator _costCalculator;
            const std::array<double, 2> _xMin{0.0, 0.0};
            const std::array<double, 2> _xMax{2.0, 2.0};
            const std::array<int, 2> _nX{2, 4};
            const double _errorFunctionalScalingFactor = 1.0;
            const core::freqInfo _freq{0.0, 10.0, 5};
            const core::grid2D _grid{_xMin, _xMax, _nX};
            const core::Sources _sources{_xMin, _xMax, 2};
            const core::Receivers _receivers{_xMin, _xMax, 2};
            const core::FrequenciesGroup _frequencies{_freq, 2000.0};
            const int _lengthOfPData = _sources.count * _frequencies.count * _receivers.count;
            double _derivativeStepSize = 0.0;
            NiceMock<forwardModels::ForwardModelMock> _forwardModel;
        };

        TEST_F(GradientDescentDirectionCalculatorTest, expectThrowNegativeDerivativeStepTest)
        {
            // Create a gradient descent calculator with derivative step size zero
            _derivativeStepSize = 0.0;

            const double pDataValue = 1.0;
            std::vector<std::complex<double>> pData(_lengthOfPData, pDataValue);
            ON_CALL(_forwardModel, getGrid).WillByDefault(ReturnRef(_grid));

            EXPECT_THROW(GradientDescentDirectionCalculator(_errorFunctionalScalingFactor, _costCalculator, &_forwardModel, _derivativeStepSize, pData),
                std::invalid_argument);

            // Create a gradient descent calculatr with negative step size
            _derivativeStepSize = -1.0;
            EXPECT_THROW(GradientDescentDirectionCalculator(_errorFunctionalScalingFactor, _costCalculator, &_forwardModel, _derivativeStepSize, pData),
                std::invalid_argument);
        }

        TEST_F(GradientDescentDirectionCalculatorTest, calculateDirectionTest)
        {
            // Create gradient descent calculator
            _derivativeStepSize = 0.1;

            const double pDataValue = 1.0;
            std::vector<std::complex<double>> pData(_lengthOfPData, pDataValue);
            ON_CALL(_forwardModel, getGrid).WillByDefault(ReturnRef(_grid));

            GradientDescentDirectionCalculator directionCalculator(_errorFunctionalScalingFactor, _costCalculator, &_forwardModel, _derivativeStepSize, pData);

            // Compute gradient descent direction

            core::dataGrid2D chiEstimate(_grid);
            const double chiEstimateValue = 2.0;
            chiEstimate = chiEstimateValue;

            std::vector<std::complex<double>> residuals(_grid.getNumberOfGridPoints(), 0.0);
            ON_CALL(_forwardModel, calculatePressureField(_)).WillByDefault([&](const core::dataGrid2D &chiEstimate) {
                return std::vector<std::complex<double>>(pData.size(), (chiEstimate.getData())[0]);
            });
            core::dataGrid2D &gDDirection = directionCalculator.calculateDirection(chiEstimate, residuals);

            // Compare gradient descent direction with expected value
            const int nrOfGridPoints = gDDirection.getNumberOfGridPoints();
            const double expectedDirection =
                -(_derivativeStepSize - 2 * (pDataValue - chiEstimateValue)) *
                (_errorFunctionalScalingFactor * _lengthOfPData);   // the initial minus sign is because our direction is more or less minus the gradient

            const std::vector<double> &gDDirectionData = gDDirection.getData();

            EXPECT_NEAR(gDDirectionData[0], expectedDirection, 0.001);
            for(int i = 1; i < nrOfGridPoints; i++)
            {
                ASSERT_DOUBLE_EQ(gDDirectionData[i], 0.0);
            }
        }

        TEST_F(GradientDescentDirectionCalculatorTest, InitializeDirectionTest)
        {
            // Create gradient descent direction calculator
            _derivativeStepSize = 1.0;

            const double pDataValue = 2.0;
            std::vector<std::complex<double>> pData(_lengthOfPData, pDataValue);
            ON_CALL(_forwardModel, getGrid).WillByDefault(ReturnRef(_grid));

            GradientDescentDirectionCalculator directionCalculator(_errorFunctionalScalingFactor, _costCalculator, &_forwardModel, _derivativeStepSize, pData);

            // Compute gradient descent direction

            core::dataGrid2D chiEstimate(_grid);
            const double chiEstimateValue = 0.0;
            chiEstimate = chiEstimateValue;

            std::vector<std::complex<double>> residuals(_grid.getNumberOfGridPoints(), 0.0);
            core::dataGrid2D gDDirection(_grid);

            ON_CALL(_forwardModel, calculatePressureField(_)).WillByDefault([&](const core::dataGrid2D &chiEstimate) {
                return std::vector<std::complex<double>>(pData.size(), (chiEstimate.getData())[0]);
            });
            gDDirection = directionCalculator.calculateDirection(chiEstimate, residuals);

            // Compare gradient descent direction with expected value
            const int nrOfGridPoints = gDDirection.getNumberOfGridPoints();
            const double expectedDirection =
                -(_derivativeStepSize - 2 * (pDataValue - chiEstimateValue)) *
                (_errorFunctionalScalingFactor * _lengthOfPData);   // the initial minus sign is because our direction is more or less minus the gradient

            const std::vector<double> &gDDirectionData = gDDirection.getData();

            EXPECT_NEAR(gDDirectionData[0], expectedDirection, 0.001);
            for(int i = 1; i < nrOfGridPoints; i++)
            {
                ASSERT_DOUBLE_EQ(gDDirectionData[i], 0.0);
            }
        }
    }   // namespace inversionMethods
}   // namespace fwi
