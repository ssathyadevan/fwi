#include "GradientDescentDirectionCalculator.h"
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

TEST(GradientDescentDirectionCalculatorTest, calculateDirectionTest)
{
    grid2D grid = getGrid();

    std::array<double, 2> xMin = {0.0, 0.0};
    std::array<double, 2> xMax = {2.0, 2.0};
    freqInfo freq(0.0, 10.0, 5);
    sources sources(xMin, xMax, 2);
    receivers receivers(xMin, xMax, 2);
    frequenciesGroup frequencies(freq, 2000.0);

    forwardModelInterface *forwardmodel;
    forwardmodel = new ForwardModelInterfaceMock(grid, sources, receivers, frequencies);

    double errorFunctionalScalingFactor = 1.0;
    double derivativeStepSize = 0.1;

    int lengthOfPData = forwardmodel->getSrc().nSrc * forwardmodel->getFreq().nFreq * forwardmodel->getRecv().nRecv;
    std::vector<std::complex<double>> pData(lengthOfPData, 1.0);
    DirectionCalculator *directionCalulator = new GradientDescentDirectionCalculator(errorFunctionalScalingFactor, forwardmodel, derivativeStepSize, pData);

    dataGrid2D chiEstimate(grid);
    std::vector<std::complex<double>> residuals(grid.getNumberOfGridPoints(), 0.0);
    dataGrid2D gDDirection(grid);
    gDDirection = directionCalulator->calculateDirection(chiEstimate, residuals);

    const int nrOfGridPoints = gDDirection.getNumberOfGridPoints();
    const double expectedDirection = static_cast<double>(lengthOfPData);
    const std::vector<double> &data = gDDirection.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], expectedDirection);
    }

    delete forwardmodel;
    delete directionCalulator;
}

TEST(GradientDescentDirectionCalculatorTest, InitializeDirectionTest)
{
    grid2D grid = getGrid();

    std::array<double, 2> xMin = {0.0, 0.0};
    std::array<double, 2> xMax = {2.0, 2.0};
    freqInfo freq(0.0, 10.0, 5);
    sources sources(xMin, xMax, 2);
    receivers receivers(xMin, xMax, 2);
    frequenciesGroup frequencies(freq, 2000.0);

    forwardModelInterface *forwardmodel;
    forwardmodel = new ForwardModelInterfaceMock(grid, sources, receivers, frequencies);

    double errorFunctionalScalingFactor = 1.0;
    double derivativeStepSize = 0.0;

    int lengthOfPData = forwardmodel->getSrc().nSrc * forwardmodel->getFreq().nFreq * forwardmodel->getRecv().nRecv;
    std::vector<std::complex<double>> pData(lengthOfPData, 2.0);
    DirectionCalculator *directionCalulator = new GradientDescentDirectionCalculator(errorFunctionalScalingFactor, forwardmodel, derivativeStepSize, pData);

    dataGrid2D chiEstimate(grid);
    std::vector<std::complex<double>> residuals(grid.getNumberOfGridPoints(), 0.0);
    dataGrid2D gDDirection(grid);
    gDDirection = directionCalulator->calculateDirection(chiEstimate, residuals);

    const int nrOfGridPoints = gDDirection.getNumberOfGridPoints();
    const double expectedDirection = 0.0;
    const std::vector<double> &data = gDDirection.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], expectedDirection);
    }

    delete forwardmodel;
    delete directionCalulator;
}
