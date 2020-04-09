#include "DirectionCalculator.h"
#include "DirectionCalculatorMock.h"
#include "forwardmodelinterfacemock.h"
#include <gtest/gtest.h>

grid2D getGrid()
{
    std::array<double, 2> xMin = {0.0, 0.0};
    std::array<double, 2> xMax = {2.0, 2.0};
    std::array<int, 2> nX = {2, 4};

    grid2D grid(xMin, xMax, nX);
    return grid;
}

forwardModelInterface *createForwardModelMock()
{
    std::array<double, 2> xMin = {0.0, 0.0};
    std::array<double, 2> xMax = {2.0, 2.0};
    freqInfo freq;
    grid2D grid = getGrid();
    sources sources(xMin, xMax, 2);
    receivers receivers(xMin, xMax, 2);
    frequenciesGroup frequencies(freq, 2000.0);

    forwardModelInterface *forwardmodel = new ForwardModelInterfaceMock(grid, sources, receivers, frequencies);
    return forwardmodel;
}

TEST(DirectionCalculatorTest, ConstructorScalingFactorTest)
{
    double errorFunctionalScalingFactor = 1.0;
    forwardModelInterface *forwardmodel = createForwardModelMock();

    DirectionCalculator *directionCalulator = new DirectionCalculatorMock(errorFunctionalScalingFactor, forwardmodel);
    double mockErrorFunctionalScalingFactor = directionCalulator->getErrorFunctionalScalingFactor();

    EXPECT_EQ(errorFunctionalScalingFactor, mockErrorFunctionalScalingFactor);

    delete forwardmodel;
    delete directionCalulator;
}
