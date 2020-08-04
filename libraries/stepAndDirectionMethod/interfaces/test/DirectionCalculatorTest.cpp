#include "DirectionCalculator.h"
#include "DirectionCalculatorMock.h"
#include "ForwardModelMock.h"
#include <gtest/gtest.h>

namespace fwi
{
    namespace inversionMethods
    {
        core::grid2D getGrid()
        {
            std::array<double, 2> xMin = {0.0, 0.0};
            std::array<double, 2> xMax = {2.0, 2.0};
            std::array<int, 2> nX = {2, 4};

            core::grid2D grid(xMin, xMax, nX);
            return grid;
        }

        TEST(DirectionCalculatorTest, ConstructorScalingFactorTest)
        {
            //            double errorFunctionalScalingFactor = 1.0;

            //            core::grid2D grid = getGrid();
            //            std::array<double, 2> xMin = {0.0, 0.0};
            //            std::array<double, 2> xMax = {2.0, 2.0};
            //            core::freqInfo freq(0.0, 10.0, 5);
            //            core::Sources sources(xMin, xMax, 2);
            //            core::Receivers receivers(xMin, xMax, 2);
            //            core::FrequenciesGroup frequencies(freq, 2000.0);

            //            forwardModels::ForwardModelInterface *forwardModel;
            //            forwardModel = new forwardModels::ForwardModelMock(grid, sources, receivers, frequencies);

            //            DirectionCalculator *directionCalculator = new DirectionCalculatorMock(errorFunctionalScalingFactor, forwardModel);
            //            double mockErrorFunctionalScalingFactor = directionCalculator->getErrorFunctionalScalingFactor();

            //            EXPECT_EQ(errorFunctionalScalingFactor, mockErrorFunctionalScalingFactor);

            //            delete forwardModel;
            //            delete directionCalculator;
        }

        TEST(DirectionCalculatorTest, ScalingFactorExceptionTest)
        {
            //            double errorFunctionalScalingFactor = -1.0;

            //            core::grid2D grid = getGrid();
            //            std::array<double, 2> xMin = {0.0, 0.0};
            //            std::array<double, 2> xMax = {2.0, 2.0};
            //            core::freqInfo freq(0.0, 10.0, 5);
            //            core::Sources sources(xMin, xMax, 2);
            //            core::Receivers receivers(xMin, xMax, 2);
            //            core::FrequenciesGroup frequencies(freq, 2000.0);

            //            forwardModels::ForwardModelInterface *forwardModel;
            //            forwardModel = new forwardModels::ForwardModelMock(grid, sources, receivers, frequencies);

            //            EXPECT_THROW(DirectionCalculatorMock(errorFunctionalScalingFactor, forwardModel), std::invalid_argument);

            //            delete forwardModel;
        }
    }   // namespace inversionMethods
}   // namespace fwi
