#include "factory.h"
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

TEST(factoryTest, createFixedStepSizeCalculatorTest)
{
    // Create a fixed step size calculator
    const std::string desiredStepSizeMethod = "fixedStepSize";
    const std::string caseFolder = "";

    StepSizeCalculator *fixedStepSizeCalculator;
    fixedStepSizeCalculator = Factory::createStepSizeCalculator(caseFolder, desiredStepSizeMethod);

    // Compute a fixed step size
    const double stepsize = fixedStepSizeCalculator->calculateStepSize();

    // Compare the fixed step size with expected value
    const double expectedStepSize = 1.0;
    EXPECT_EQ(stepsize, expectedStepSize);

    delete fixedStepSizeCalculator;
}

TEST(factoryTest, expectThrowStepSizeCalculatorTest)
{
    // Create a not existing step size calculator
    const std::string desiredStepSizeMethod = "";
    const std::string caseFolder = "";

    EXPECT_THROW(Factory::createStepSizeCalculator(caseFolder, desiredStepSizeMethod), std::invalid_argument);
}

TEST(factoryTest, createConjugateGradientDirectionCalculatorTest)
{
    // Create a forwardmodel
    std::array<double, 2> xMin = {0.0, 0.0};
    std::array<double, 2> xMax = {2.0, 2.0};
    freqInfo freq;
    grid2D grid = getGrid();
    sources sources(xMin, xMax, 2);
    receivers receivers(xMin, xMax, 2);
    frequenciesGroup frequencies(freq, 2000.0);

    forwardModelInterface *forwardmodel = new ForwardModelInterfaceMock(grid, sources, receivers, frequencies);

    // Create measurement data
    const int lengthOfPData = forwardmodel->getSrc().nSrc * forwardmodel->getRecv().nRecv * forwardmodel->getFreq().nFreq;
    const std::vector<std::complex<double>> pData(lengthOfPData, 1.0);

    // Create a conjugate gradient descent direction calculator
    const std::string desiredStepSizeMethod = "conjugateGradientDirection";
    const std::string caseFolder = "";
    DirectionCalculator *cGDirectionCalculator;
    cGDirectionCalculator = Factory::createDirectionCalculator(caseFolder, desiredStepSizeMethod, forwardmodel, pData);

    // Compare error functional scaling factor with expected value
    const double errorFunctionScalingFactor = cGDirectionCalculator->getErrorFunctionalScalingFactor();
    const double expectedErrorFunctionalScalingFactor = 1.0 / (lengthOfPData);

    EXPECT_EQ(errorFunctionScalingFactor, expectedErrorFunctionalScalingFactor);

    // Compute conjugate gradient direction
    dataGrid2D cGDirection(grid);
    dataGrid2D chi(grid);
    std::vector<std::complex<double>> residuals(grid.getNumberOfGridPoints(), 1.0);
    cGDirection = cGDirectionCalculator->calculateDirection(chi, residuals);

    // Compare conjugate gradient direction with the expected value
    const double expectedKappaTimesresidual = 5.0;
    const int nrOfGridPoints = cGDirection.getNumberOfGridPoints();
    const double expectedDirection = expectedErrorFunctionalScalingFactor * expectedKappaTimesresidual;

    const std::vector<double> &cGDirectionData = cGDirection.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(cGDirectionData[i], expectedDirection);
    }

    delete cGDirectionCalculator;
    delete forwardmodel;
}

TEST(factoryTest, createGradientDescenttDirectionCalculatorTest)
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

    // Create measurement data
    int lengthOfPData = forwardmodel->getSrc().nSrc * forwardmodel->getRecv().nRecv * forwardmodel->getFreq().nFreq;
    const double pDataValue = 1.0;
    const std::vector<std::complex<double>> pData(lengthOfPData, pDataValue);

    // Create gradient descent direction calculator
    const std::string desiredStepSizeMethod = "gradientDescentDirection";
    const std::string caseFolder = "";

    DirectionCalculator *gDDirectionCalculator;
    gDDirectionCalculator = Factory::createDirectionCalculator(caseFolder, desiredStepSizeMethod, forwardmodel, pData);

    // Compare error functional scaling factor with the expected value
    const double errorFunctionalScalingFactor = gDDirectionCalculator->getErrorFunctionalScalingFactor();
    const double expectedErrorFunctionalScalingFactor = 1.0 / (lengthOfPData);

    EXPECT_EQ(errorFunctionalScalingFactor, expectedErrorFunctionalScalingFactor);

    // Compute gradient descent direction
    dataGrid2D chiEstimate(grid);
    const double chiEstimateValue = 2.0;
    chiEstimate = chiEstimateValue;

    std::vector<std::complex<double>> residuals(grid.getNumberOfGridPoints(), 0.0);
    dataGrid2D gDDirection(grid);
    gDDirection = gDDirectionCalculator->calculateDirection(chiEstimate, residuals);

    // Compare gradient descent direction with expected value
    const double expectedDerivativeStepSize = 0.1;
    const int nrOfGridPoints = gDDirection.getNumberOfGridPoints();
    const double expectedDirection = (expectedDerivativeStepSize - 2 * (pDataValue - chiEstimateValue)) * (errorFunctionalScalingFactor * lengthOfPData);

    const std::vector<double> &gDDirectionData = gDDirection.getData();

    EXPECT_NEAR(gDDirectionData[0], expectedDirection, 0.001);
    for(int i = 1; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(gDDirectionData[i], 0.0);
    }

    delete gDDirectionCalculator;
    delete forwardmodel;
}

TEST(factoryTest, expectThrowDirectionCalculatorTest)
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

    // Create measurement data
    const int lengthOfPData = forwardmodel->getSrc().nSrc * forwardmodel->getRecv().nRecv * forwardmodel->getFreq().nFreq;
    const std::vector<std::complex<double>> pData(lengthOfPData, 1.0);

    // Create a not existing direction calculator
    const std::string desiredStepSizeMethod = "";
    const std::string caseFolder = "";

    EXPECT_THROW(Factory::createDirectionCalculator(caseFolder, desiredStepSizeMethod, forwardmodel, pData), std::invalid_argument);

    delete forwardmodel;
}
