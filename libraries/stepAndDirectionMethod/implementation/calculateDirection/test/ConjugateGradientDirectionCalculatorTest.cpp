#include "ConjugateGradientDirectionCalculator.h"
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

TEST(ConjugateGradientDirectionCalculatorTest, calculateDirectionTest)
{
    grid2D grid = getGrid();
    double errorFunctionScalingFactor = 1.0;

    forwardModelInterface *forwardmodel = createForwardModelMock();
    DirectionCalculator *directionCalulator = new ConjugateGradientDirectionCalculator(errorFunctionScalingFactor, forwardmodel);

    dataGrid2D cGDirection(grid);
    dataGrid2D chi(grid);
    std::vector<std::complex<double>> residuals(grid.getNumberOfGridPoints(), 1.0);
    cGDirection = directionCalulator->calculateDirection(chi, residuals);

    const int nrOfGridPoints = cGDirection.getNumberOfGridPoints();
    const std::vector<double> &data = cGDirection.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], 0.0);
    }

    delete forwardmodel;
    delete directionCalulator;
}
