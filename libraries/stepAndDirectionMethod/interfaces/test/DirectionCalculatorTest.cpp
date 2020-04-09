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

TEST(DirectionCalculatorTest, ConstructorGridTest)
{
    grid2D grid = getGrid();

    double errorFunctionalScalingFactor = 1.0;

    forwardModelInterface *forwardmodel = createForwardModelMock();
    DirectionCalculator *directionCalulator = new DirectionCalculatorMock(grid, errorFunctionalScalingFactor, forwardmodel);
    grid2D mockDirectionGrid = directionCalulator->getGrid();

    EXPECT_EQ(grid.getNumberOfGridPoints(), mockDirectionGrid.getNumberOfGridPoints());
    EXPECT_DOUBLE_EQ(grid.getGridStart()[0], mockDirectionGrid.getGridStart()[0]);
    EXPECT_DOUBLE_EQ(grid.getGridStart()[1], mockDirectionGrid.getGridStart()[1]);
    EXPECT_DOUBLE_EQ(grid.getGridEnd()[0], mockDirectionGrid.getGridEnd()[0]);
    EXPECT_DOUBLE_EQ(grid.getGridEnd()[1], mockDirectionGrid.getGridEnd()[1]);
    EXPECT_DOUBLE_EQ(grid.getCellVolume(), mockDirectionGrid.getCellVolume());
    EXPECT_DOUBLE_EQ(grid.getDomainArea(), mockDirectionGrid.getDomainArea());
    EXPECT_DOUBLE_EQ(grid.getCellDimensions()[0], mockDirectionGrid.getCellDimensions()[0]);
    EXPECT_DOUBLE_EQ(grid.getCellDimensions()[1], mockDirectionGrid.getCellDimensions()[1]);
    EXPECT_DOUBLE_EQ(grid.getGridDimensions()[0], mockDirectionGrid.getGridDimensions()[0]);
    EXPECT_DOUBLE_EQ(grid.getGridDimensions()[1], mockDirectionGrid.getGridDimensions()[1]);

    delete forwardmodel;
    delete directionCalulator;
}

TEST(DirectionCalculatorTest, ConstructorScalingFactorTest)
{
    grid2D grid = getGrid();

    double errorFunctionalScalingFactor = 1.0;
    forwardModelInterface *forwardmodel = createForwardModelMock();

    DirectionCalculator *directionCalulator = new DirectionCalculatorMock(grid, errorFunctionalScalingFactor, forwardmodel);
    double mockErrorFunctionalScalingFactor = directionCalulator->getErrorFunctionalScalingFactor();

    EXPECT_EQ(errorFunctionalScalingFactor, mockErrorFunctionalScalingFactor);

    delete forwardmodel;
    delete directionCalulator;
}
