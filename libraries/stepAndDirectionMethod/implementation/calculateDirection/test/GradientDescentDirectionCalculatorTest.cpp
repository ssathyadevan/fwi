#include "GradientDescentDirectionCalculator.h"
#include "ForwardModelMock.h"
#include <gtest/gtest.h>

namespace fwi
{
    namespace inversionMethods
    {
        class GradientDescentDirectionCalculatorTest : public ::testing::Test
        {
        public:
            forwardModels::ForwardModelInterface *_forwardmodel;
            const core::CostFunctionCalculator _costCalculator;
            std::array<double, 2> _xMin, _xMax;
            std::array<int, 2> _nX;
            int _lengthOfPData;
            double _errorFunctionalScalingFactor;

        protected:
            void SetUp() override
            {
                _xMin = std::array<double, 2>{0.0, 0.0};
                _xMax = std::array<double, 2>{2.0, 2.0};
                _nX = std::array<int, 2>{2, 4};
                core::freqInfo freq(0.0, 10.0, 5);
                core::Sources sources(_xMin, _xMax, 2);
                core::Receivers receivers(_xMin, _xMax, 2);
                core::FrequenciesGroup frequencies(freq, 2000.0);
                core::grid2D grid(_xMin, _xMax, _nX);
                _forwardmodel = new forwardModels::ForwardModelMock(grid, sources, receivers, frequencies);
                _lengthOfPData = _forwardmodel->getSource().count * _forwardmodel->getFreq().count * _forwardmodel->getReceiver().count;
                _errorFunctionalScalingFactor = 1.0;
            }

            void TearDown() override { delete _forwardmodel; }
        };

        TEST_F(GradientDescentDirectionCalculatorTest, expectThrowNegativeDerivativeStepTest)
        {
            // Create a gradient descent calculator with derivative step size zero
            double derivativeStepSize = 0.0;

            const double pDataValue = 1.0;
            std::vector<std::complex<double>> pData(_lengthOfPData, pDataValue);
            EXPECT_THROW(GradientDescentDirectionCalculator(_errorFunctionalScalingFactor, _costCalculator, _forwardmodel, derivativeStepSize, pData),
                std::invalid_argument);

            // Create a gradient descent calculatr with negative step size
            derivativeStepSize = -1.0;
            EXPECT_THROW(GradientDescentDirectionCalculator(_errorFunctionalScalingFactor, _costCalculator, _forwardmodel, derivativeStepSize, pData),
                std::invalid_argument);
        }

        TEST_F(GradientDescentDirectionCalculatorTest, calculateDirectionTest)
        {
            // Create gradient descent calculator
            double derivativeStepSize = 0.1;

            const double pDataValue = 1.0;
            std::vector<std::complex<double>> pData(_lengthOfPData, pDataValue);
            DirectionCalculator *directionCalculator =
                new GradientDescentDirectionCalculator(_errorFunctionalScalingFactor, _costCalculator, _forwardmodel, derivativeStepSize, pData);

            // Compute gradient descent direction
            core::grid2D grid(_xMin, _xMax, _nX);
            core::dataGrid2D chiEstimate(grid);
            const double chiEstimateValue = 2.0;
            chiEstimate = chiEstimateValue;

            std::vector<std::complex<double>> residuals(grid.getNumberOfGridPoints(), 0.0);

            core::dataGrid2D const *gDDirection = NULL;
            // core::dataGrid2D gDDirection(grid);
            gDDirection = &directionCalculator->calculateDirection(chiEstimate, residuals);

            // Compare gradient descent direction with expected value
            const int nrOfGridPoints = gDDirection->getNumberOfGridPoints();
            const double expectedDirection =
                -(derivativeStepSize - 2 * (pDataValue - chiEstimateValue)) *
                (_errorFunctionalScalingFactor * _lengthOfPData);   // the initial minus sign is because our direction is more or less minus the gradient

            const std::vector<double> &gDDirectionData = gDDirection->getData();

            EXPECT_NEAR(gDDirectionData[0], expectedDirection, 0.001);
            for(int i = 1; i < nrOfGridPoints; i++)
            {
                ASSERT_DOUBLE_EQ(gDDirectionData[i], 0.0);
            }

            delete directionCalculator;
        }

        TEST_F(GradientDescentDirectionCalculatorTest, InitializeDirectionTest)
        {
            // Create gradient descent direction calculator
            const double derivativeStepSize = 1.0;

            const double pDataValue = 2.0;
            std::vector<std::complex<double>> pData(_lengthOfPData, pDataValue);
            DirectionCalculator *directionCalculator =
                new GradientDescentDirectionCalculator(_errorFunctionalScalingFactor, _costCalculator, _forwardmodel, derivativeStepSize, pData);

            // Compute gradient descent direction
            core::grid2D grid(_xMin, _xMax, _nX);
            core::dataGrid2D chiEstimate(grid);
            const double chiEstimateValue = 0.0;
            chiEstimate = chiEstimateValue;

            std::vector<std::complex<double>> residuals(grid.getNumberOfGridPoints(), 0.0);
            core::dataGrid2D gDDirection(grid);
            gDDirection = directionCalculator->calculateDirection(chiEstimate, residuals);

            // Compare gradient descent direction with expected value
            const int nrOfGridPoints = gDDirection.getNumberOfGridPoints();
            const double expectedDirection =
                -(derivativeStepSize - 2 * (pDataValue - chiEstimateValue)) *
                (_errorFunctionalScalingFactor * _lengthOfPData);   // the initial minus sign is because our direction is more or less minus the gradient

            const std::vector<double> &gDDirectionData = gDDirection.getData();

            EXPECT_NEAR(gDDirectionData[0], expectedDirection, 0.001);
            for(int i = 1; i < nrOfGridPoints; i++)
            {
                ASSERT_DOUBLE_EQ(gDDirectionData[i], 0.0);
            }

            delete directionCalculator;
        }
    }   // namespace inversionMethods
}   // namespace fwi
