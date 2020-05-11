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

TEST(GradientDescentDirectionCalculatorTest, expectThrowNegativeDerivativeStepTest)
{
    // Create forward model
    grid2D grid = getGrid();
    std::array<double, 2> xMin = {0.0, 0.0};
    std::array<double, 2> xMax = {2.0, 2.0};
    freqInfo freq(0.0, 10.0, 5);
    sources sources(xMin, xMax, 2);
    receivers receivers(xMin, xMax, 2);
    frequenciesGroup frequencies(freq, 2000.0);

    forwardModelInterface *forwardmodel;
    forwardmodel = new ForwardModelInterfaceMock(grid, sources, receivers, frequencies);

    // Create a gradient descent calculator with derivative step size zero
    double errorFunctionalScalingFactor = 1.0;
    double derivativeStepSize = 0.0;

    int lengthOfPData = forwardmodel->getSrc().nSrc * forwardmodel->getFreq().nFreq * forwardmodel->getRecv().nRecv;
    const double pDataValue = 1.0;
    std::vector<std::complex<double>> pData(lengthOfPData, pDataValue);
    EXPECT_THROW(GradientDescentDirectionCalculator(errorFunctionalScalingFactor, forwardmodel, derivativeStepSize, pData), std::invalid_argument);

    // Create a gradient descent calculatr with negative step size
    derivativeStepSize = -1.0;
    EXPECT_THROW(GradientDescentDirectionCalculator(errorFunctionalScalingFactor, forwardmodel, derivativeStepSize, pData), std::invalid_argument);

    delete forwardmodel;
}

TEST(GradientDescentDirectionCalculatorTest, calculateDirectionTest)
{
    // Create forwardmodel
    grid2D grid = getGrid();
    std::array<double, 2> xMin = {0.0, 0.0};
    std::array<double, 2> xMax = {2.0, 2.0};
    freqInfo freq(0.0, 10.0, 5);
    sources sources(xMin, xMax, 2);
    receivers receivers(xMin, xMax, 2);
    frequenciesGroup frequencies(freq, 2000.0);

    forwardModelInterface *forwardmodel;
    forwardmodel = new ForwardModelInterfaceMock(grid, sources, receivers, frequencies);

    // Create gradient descent calculator
    double errorFunctionalScalingFactor = 1.0;
    double derivativeStepSize = 0.1;

    int lengthOfPData = forwardmodel->getSrc().nSrc * forwardmodel->getFreq().nFreq * forwardmodel->getRecv().nRecv;
    const double pDataValue = 1.0;
    std::vector<std::complex<double>> pData(lengthOfPData, pDataValue);
    DirectionCalculator *directionCalulator = new GradientDescentDirectionCalculator(errorFunctionalScalingFactor, forwardmodel, derivativeStepSize, pData);

    // Compute gradient descent direction
    dataGrid2D chiEstimate(grid);
    const double chiEstimateValue = 2.0;
    chiEstimate = chiEstimateValue;

    std::vector<std::complex<double>> residuals(grid.getNumberOfGridPoints(), 0.0);

    dataGrid2D const *gDDirection = NULL;
    // dataGrid2D gDDirection(grid);
    gDDirection = &directionCalulator->calculateDirection(chiEstimate, residuals);

    // Compare gradient descent direction with expected value
    const int nrOfGridPoints = gDDirection->getNumberOfGridPoints();
    const double expectedDirection =
        -(derivativeStepSize - 2 * (pDataValue - chiEstimateValue)) *
        (errorFunctionalScalingFactor * lengthOfPData);   // the initial minus sign is because our direction is more or less minus the gradient

    const std::vector<double> &gDDirectionData = gDDirection->getData();

    EXPECT_NEAR(gDDirectionData[0], expectedDirection, 0.001);
    for(int i = 1; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(gDDirectionData[i], 0.0);
    }

    delete forwardmodel;
    delete directionCalulator;
}

TEST(GradientDescentDirectionCalculatorTest, InitializeDirectionTest)
{
    // Create forwardmodel
    grid2D grid = getGrid();
    std::array<double, 2> xMin = {0.0, 0.0};
    std::array<double, 2> xMax = {2.0, 2.0};
    freqInfo freq(0.0, 10.0, 5);
    sources sources(xMin, xMax, 2);
    receivers receivers(xMin, xMax, 2);
    frequenciesGroup frequencies(freq, 2000.0);

    forwardModelInterface *forwardmodel;
    forwardmodel = new ForwardModelInterfaceMock(grid, sources, receivers, frequencies);

    // Create gradient descent direction calculator
    const double errorFunctionalScalingFactor = 1.0;
    const double derivativeStepSize = 1.0;

    const int lengthOfPData = forwardmodel->getSrc().nSrc * forwardmodel->getFreq().nFreq * forwardmodel->getRecv().nRecv;
    const double pDataValue = 2.0;
    std::vector<std::complex<double>> pData(lengthOfPData, pDataValue);
    DirectionCalculator *directionCalulator = new GradientDescentDirectionCalculator(errorFunctionalScalingFactor, forwardmodel, derivativeStepSize, pData);

    // Compute gradient descent direction
    dataGrid2D chiEstimate(grid);
    const double chiEstimateValue = 0.0;
    chiEstimate = chiEstimateValue;

    std::vector<std::complex<double>> residuals(grid.getNumberOfGridPoints(), 0.0);
    dataGrid2D gDDirection(grid);
    gDDirection = directionCalulator->calculateDirection(chiEstimate, residuals);

    // Compare gradient descent direction with expected value
    const int nrOfGridPoints = gDDirection.getNumberOfGridPoints();
    const double expectedDirection =
        -(derivativeStepSize - 2 * (pDataValue - chiEstimateValue)) *
        (errorFunctionalScalingFactor * lengthOfPData);   // the initial minus sign is because our direction is more or less minus the gradient

    const std::vector<double> &gDDirectionData = gDDirection.getData();

    EXPECT_NEAR(gDDirectionData[0], expectedDirection, 0.001);
    for(int i = 1; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(gDDirectionData[i], 0.0);
    }

    delete forwardmodel;
    delete directionCalulator;
}
