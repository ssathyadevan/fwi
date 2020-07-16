#include "DummyDirectionCalculator.h"
#include "forwardmodelinterfacemock.h"
#include <gtest/gtest.h>

namespace fwi
{
    namespace inversionMethods
    {
        // Review: If getGrid() is meant to be used only in this, file please
        // put the definition inside a nameless namespace so that it doesn't
        // pollute the namespace
        core::grid2D getGrid()
        {
            std::array<double, 2> xMin = {0.0, 0.0};
            std::array<double, 2> xMax = {2.0, 2.0};
            std::array<int, 2> nX = {2, 4};

            core::grid2D grid(xMin, xMax, nX);
            return grid;
        }

        // Review: the test itself looks huge if we consider that it tests that
        // the class is doing nothing (this is related to how forwardModel and Direction Calculator classes are entangled
        //  I know it is out of scope but it is something we should think about)

        // Review: I cant really say if this test is fulfilling its purpose because
        // I am not sure about the purpose of DummyDirectionCalculator
        TEST(DummyDirectionCalculatorTest, calculateDirectionTest)
        {
            // Arrange
            core::grid2D grid = getGrid();
            // Review: if you need xMin and xMax here again it means that
            // by also defining nX you could do grid(xMin, xMax, nX) here instead
            // of using a separate function that only calls core::grid2D constructor
			
			// Review: if these are all initialization parameters please make const
            std::array<double, 2> xMin = {0.0, 0.0};
            std::array<double, 2> xMax = {2.0, 2.0};
            core::freqInfo freq(0.0, 10.0, 5);
            core::Sources sources(xMin, xMax, 2);
            core::Receivers receivers(xMin, xMax, 2);
            core::FrequenciesGroup frequencies(freq, 2000.0);

			// Review: forwardModel could also be a const ptr 
            forwardModels::forwardModelInterface *forwardModel;
            forwardModel = new forwardModels::ForwardModelInterfaceMock(grid, sources, receivers, frequencies);

            double errorFunctionalScalingFactor = 1.0;
			// Review: directionCalculator could also be a const ptr 
            DirectionCalculator *directionCalculator = new DummyDirectionCalculator(errorFunctionalScalingFactor, forwardModel);
			// Review: chiEstimate should be const
            core::dataGrid2D chiEstimate(grid);
			// Review: residuals should be const
            std::vector<std::complex<double>> residuals(grid.getNumberOfGridPoints(), 0.0);

            // Act
            std::vector<double> const dummyDirection = directionCalculator->calculateDirection(chiEstimate, residuals).getData();

            // Assert
            for(int i = 0; i < (int)dummyDirection.size(); i++)
            {
                ASSERT_DOUBLE_EQ(dummyDirection[i], 0.0);
            }

            delete forwardModel;
            delete directionCalculator;
        }
    }   // namespace inversionMethods
}   // namespace fwi
