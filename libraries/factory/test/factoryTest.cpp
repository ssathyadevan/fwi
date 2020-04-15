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

TEST(factoryTest, createFixedStepSizeCalculatorTest)
{
    const std::string desiredStepSizeMethod = "fixedStepSize";
    const std::string caseFolder = "";

    StepSizeCalculator *stepSizeCalculator;
    stepSizeCalculator = Factory::createStepSizeCalculator(caseFolder, desiredStepSizeMethod);

    const double stepsize = stepSizeCalculator->calculateStepSize();
    const double expectedStepSize = 1.0;

    EXPECT_EQ(stepsize, expectedStepSize);

    delete stepSizeCalculator;
}

TEST(factoryTest, createConjugateGradientDirectionCalculatorTest)
{
    const std::string desiredStepSizeMethod = "fixedStepSize";
    const std::string caseFolder = "";

    forwardModelInterface *forwardmodel;
    forwardmodel = createForwardModelMock();

    const int nTotal = forwardmodel->getSrc().nSrc * forwardmodel->getRecv().nRecv * forwardmodel->getFreq().nFreq;
    const std::vector<std::complex<double>> pData(nTotal, 1.0);

    DirectionCalculator *directionCalculator;
    directionCalculator = Factory::createDirectionCalculator(caseFolder, desiredStepSizeMethod, forwardmodel, pData);

    const double errorFunctionScalingFactor = directionCalculator->getErrorFunctionalScalingFactor();
    const double expectedErrorFunctionalScalingFactor = 1.0 / (nTotal);

    EXPECT_EQ(errorFunctionScalingFactor, expectedErrorFunctionalScalingFactor);

    delete directionCalculator;
    delete forwardmodel;
}
