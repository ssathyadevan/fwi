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

TEST(factoryTest, expectThrowMissingForwardModelTest)
{
    // Create null pointer to forwardmodel
    forwardModelInterface *forwardModel = nullptr;

    // Create a fixed step size with conjugate gradient method
    const std::string desiredStepSizeMethod = "fixedStepSize";
    const std::string desiredDirectionMethod = "conjugateGradientDirection";
    const std::string caseFolder = "";

    std::array<double, 2> xMin = {0.0, 0.0};
    std::array<double, 2> xMax = {2.0, 2.0};
    freqInfo freq(1.0, 2.0, 2);
    sources sources(xMin, xMax, 2);
    receivers receivers(xMin, xMax, 2);
    frequenciesGroup frequencies(freq, 2000.0);
    // Create measurement data
    const int lengthOfPData = sources.nSrc * receivers.nRecv * frequencies.nFreq;
    const std::vector<std::complex<double>> pData(lengthOfPData, 1.0);

    Factory factory;
    EXPECT_THROW(
        factory.createStepAndDirectionReconstructor(caseFolder, forwardModel, desiredStepSizeMethod, desiredDirectionMethod, pData), std::invalid_argument);
}

TEST(factoryTest, createFixedStepSizeConjugateGradientMethodTest)
{
    // Create forwardmodel
    std::array<double, 2> xMin = {0.0, 0.0};
    std::array<double, 2> xMax = {2.0, 2.0};
    freqInfo freq(1.0, 2.0, 2);
    grid2D grid = getGrid();
    sources sources(xMin, xMax, 2);
    receivers receivers(xMin, xMax, 2);
    frequenciesGroup frequencies(freq, 2000.0);

    ForwardModelInterfaceMock forwardModel(grid, sources, receivers, frequencies);

    // Create a fixed step size with conjugate gradient method
    const std::string desiredStepSizeMethod = "fixedStepSize";
    const std::string desiredDirectionMethod = "conjugateGradientDirection";
    const std::string caseFolder = "";

    // Create measurement data
    const int lengthOfPData = forwardModel.getSrc().nSrc * forwardModel.getRecv().nRecv * forwardModel.getFreq().nFreq;
    const std::vector<std::complex<double>> pData(lengthOfPData, 1.0);

    Factory factory;
    StepAndDirectionReconstructor *reconstructor;
    reconstructor = factory.createStepAndDirectionReconstructor(caseFolder, &forwardModel, desiredStepSizeMethod, desiredDirectionMethod, pData);
    EXPECT_FALSE(reconstructor == nullptr);
}

TEST(factoryTest, expectThrowStepSizeCalculatorTest)
{
    // Create forwardmodel
    std::array<double, 2> xMin = {0.0, 0.0};
    std::array<double, 2> xMax = {2.0, 2.0};
    freqInfo freq(1.0, 2.0, 2);
    grid2D grid = getGrid();
    sources sources(xMin, xMax, 2);
    receivers receivers(xMin, xMax, 2);
    frequenciesGroup frequencies(freq, 2000.0);

    ForwardModelInterfaceMock forwardModel(grid, sources, receivers, frequencies);

    // Create a not existing step size calculator
    const std::string desiredStepSizeMethod = "";
    const std::string desiredDirectionMethod = "conjugateGradientDirection";
    const std::string caseFolder = "";

    // Create measurement data
    const int lengthOfPData = forwardModel.getSrc().nSrc * forwardModel.getRecv().nRecv * forwardModel.getFreq().nFreq;
    const std::vector<std::complex<double>> pData(lengthOfPData, 1.0);

    Factory factory;
    EXPECT_THROW(
        factory.createStepAndDirectionReconstructor(caseFolder, &forwardModel, desiredStepSizeMethod, desiredDirectionMethod, pData), std::invalid_argument);
}

TEST(factoryTest, createFixedStepSizeGradientDescentMethodTest)
{
    // Create forwardmodel
    std::array<double, 2> xMin = {0.0, 0.0};
    std::array<double, 2> xMax = {2.0, 2.0};
    freqInfo freq(1.0, 2.0, 2);
    grid2D grid = getGrid();
    sources sources(xMin, xMax, 2);
    receivers receivers(xMin, xMax, 2);
    frequenciesGroup frequencies(freq, 2000.0);

    ForwardModelInterfaceMock forwardModel(grid, sources, receivers, frequencies);

    // Create measurement data
    const int lengthOfPData = forwardModel.getSrc().nSrc * forwardModel.getRecv().nRecv * forwardModel.getFreq().nFreq;
    const std::vector<std::complex<double>> pData(lengthOfPData, 1.0);

    // Create a fixed step with conjugate gradient descent method
    const std::string desiredStepSizeMethod = "fixedStepSize";
    const std::string desiredDirectionMethod = "gradientDescentDirection";
    const std::string caseFolder = "";

    Factory factory;
    StepAndDirectionReconstructor *reconstructor;
    reconstructor = factory.createStepAndDirectionReconstructor(caseFolder, &forwardModel, desiredStepSizeMethod, desiredDirectionMethod, pData);
    EXPECT_FALSE(reconstructor == nullptr);
}

TEST(factoryTest, expectThrowDirectionCalculatorTest)
{
    // Create forwardmodel
    std::array<double, 2> xMin = {0.0, 0.0};
    std::array<double, 2> xMax = {2.0, 2.0};
    freqInfo freq(1.0, 2.0, 2);
    grid2D grid = getGrid();
    sources sources(xMin, xMax, 2);
    receivers receivers(xMin, xMax, 2);
    frequenciesGroup frequencies(freq, 2000.0);

    ForwardModelInterfaceMock forwardModel(grid, sources, receivers, frequencies);

    // Create measurement data
    const int lengthOfPData = forwardModel.getSrc().nSrc * forwardModel.getRecv().nRecv * forwardModel.getFreq().nFreq;
    ;
    const std::vector<std::complex<double>> pData(lengthOfPData, 1.0);

    // Create a not existing direction calculator
    const std::string desiredStepSizeMethod = "fixedStepSize";
    const std::string desiredDirectionMethod = "";
    const std::string caseFolder = "";

    Factory factory;
    EXPECT_THROW(
        factory.createStepAndDirectionReconstructor(caseFolder, &forwardModel, desiredStepSizeMethod, desiredDirectionMethod, pData), std::invalid_argument);
}
