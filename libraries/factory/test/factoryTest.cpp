#include "factory.h"
#include "ForwardModelMock.h"
#include <gtest/gtest.h>

namespace fwi
{
    core::grid2D getGrid()
    {
        std::array<double, 2> xMin = {0.0, 0.0};
        std::array<double, 2> xMax = {2.0, 2.0};
        std::array<int, 2> nX = {2, 4};

        core::grid2D grid(xMin, xMax, nX);
        return grid;
    }

    inversionMethods::StepAndDirectionReconstructorInput getStepAndDirectionInput()
    {
        double tolerance = 0.01;
        double startChi = 0.0;
        int maxIterations = 10;

        double initStepiSze = 1.0;
        double slope = -0.01;

        double derivativeStepSize = 1.0;

        bool doRegression = false;
        return inversionMethods::StepAndDirectionReconstructorInput{
            {tolerance, startChi, maxIterations}, {initStepiSze, slope}, {derivativeStepSize}, doRegression};
    }

    TEST(factoryTest, expectThrowMissingForwardModelTest)
    {
        // Create null pointer to forwardmodel
        forwardModels::forwardModelInterface *forwardModel = nullptr;

        // Create a fixed step size with conjugate gradient method
        const std::string desiredStepSizeMethod = "FixedStepSize";
        const std::string desiredDirectionMethod = "ConjugateGradientDirection";

        std::array<double, 2> xMin = {0.0, 0.0};
        std::array<double, 2> xMax = {2.0, 2.0};
        core::freqInfo freq(1.0, 2.0, 2);
        core::Sources sources(xMin, xMax, 2);
        core::Receivers receivers(xMin, xMax, 2);
        core::FrequenciesGroup frequencies(freq, 2000.0);
        // Create measurement data
        const int lengthOfPData = sources.count * receivers.count * frequencies.count;
        const std::vector<std::complex<double>> pData(lengthOfPData, 1.0);

        inversionMethods::StepAndDirectionReconstructorInput stepAndDirectionInput = getStepAndDirectionInput();

        Factory factory;
        EXPECT_THROW(factory.createStepAndDirectionReconstructor(stepAndDirectionInput, forwardModel, desiredStepSizeMethod, desiredDirectionMethod, pData),
            std::invalid_argument);
    }

    TEST(factoryTest, createFixedStepSizeConjugateGradientMethodTest)
    {
        // Create forwardmodel
        std::array<double, 2> xMin = {0.0, 0.0};
        std::array<double, 2> xMax = {2.0, 2.0};
        core::freqInfo freq(1.0, 2.0, 2);
        core::grid2D grid = getGrid();
        core::Sources sources(xMin, xMax, 2);
        core::Receivers receivers(xMin, xMax, 2);
        core::FrequenciesGroup frequencies(freq, 2000.0);

        forwardModels::ForwardModelMock forwardModel(grid, sources, receivers, frequencies);

        // Create a fixed step size with conjugate gradient method
        const std::string desiredStepSizeMethod = "FixedStepSize";
        const std::string desiredDirectionMethod = "ConjugateGradientDirection";

        // Create measurement data
        const int lengthOfPData = forwardModel.getSource().count * forwardModel.getReceiver().count * forwardModel.getFreq().count;
        const std::vector<std::complex<double>> pData(lengthOfPData, 1.0);

        inversionMethods::StepAndDirectionReconstructorInput stepAndDirectionInput = getStepAndDirectionInput();

        Factory factory;
        inversionMethods::StepAndDirectionReconstructor *reconstructor;
        reconstructor = factory.createStepAndDirectionReconstructor(stepAndDirectionInput, &forwardModel, desiredStepSizeMethod, desiredDirectionMethod, pData);
        EXPECT_FALSE(reconstructor == nullptr);
    }

    TEST(factoryTest, expectThrowStepSizeCalculatorTest)
    {
        // Create forwardmodel
        std::array<double, 2> xMin = {0.0, 0.0};
        std::array<double, 2> xMax = {2.0, 2.0};
        core::freqInfo freq(1.0, 2.0, 2);
        core::grid2D grid = getGrid();
        core::Sources sources(xMin, xMax, 2);
        core::Receivers receivers(xMin, xMax, 2);
        core::FrequenciesGroup frequencies(freq, 2000.0);

        forwardModels::ForwardModelMock forwardModel(grid, sources, receivers, frequencies);

        // Create a not existing step size calculator
        const std::string desiredStepSizeMethod = "";
        const std::string desiredDirectionMethod = "ConjugateGradientDirection";

        // Create measurement data
        const int lengthOfPData = forwardModel.getSource().count * forwardModel.getReceiver().count * forwardModel.getFreq().count;
        const std::vector<std::complex<double>> pData(lengthOfPData, 1.0);

        inversionMethods::StepAndDirectionReconstructorInput stepAndDirectionInput = getStepAndDirectionInput();

        Factory factory;
        EXPECT_THROW(factory.createStepAndDirectionReconstructor(stepAndDirectionInput, &forwardModel, desiredStepSizeMethod, desiredDirectionMethod, pData),
            std::invalid_argument);
    }

    TEST(factoryTest, createFixedStepSizeGradientDescentMethodTest)
    {
        // Create forwardmodel
        std::array<double, 2> xMin = {0.0, 0.0};
        std::array<double, 2> xMax = {2.0, 2.0};
        core::freqInfo freq(1.0, 2.0, 2);
        core::grid2D grid = getGrid();
        core::Sources sources(xMin, xMax, 2);
        core::Receivers receivers(xMin, xMax, 2);
        core::FrequenciesGroup frequencies(freq, 2000.0);

        forwardModels::ForwardModelMock forwardModel(grid, sources, receivers, frequencies);

        // Create measurement data
        const int lengthOfPData = forwardModel.getSource().count * forwardModel.getReceiver().count * forwardModel.getFreq().count;
        const std::vector<std::complex<double>> pData(lengthOfPData, 1.0);

        // Create a fixed step with conjugate gradient descent method
        const std::string desiredStepSizeMethod = "FixedStepSize";
        const std::string desiredDirectionMethod = "GradientDescentDirection";

        inversionMethods::StepAndDirectionReconstructorInput stepAndDirectionInput = getStepAndDirectionInput();

        Factory factory;
        inversionMethods::StepAndDirectionReconstructor *reconstructor;
        reconstructor = factory.createStepAndDirectionReconstructor(stepAndDirectionInput, &forwardModel, desiredStepSizeMethod, desiredDirectionMethod, pData);
        EXPECT_FALSE(reconstructor == nullptr);
    }

    TEST(factoryTest, expectThrowDirectionCalculatorTest)
    {
        // Create forwardmodel
        std::array<double, 2> xMin = {0.0, 0.0};
        std::array<double, 2> xMax = {2.0, 2.0};
        core::freqInfo freq(1.0, 2.0, 2);
        core::grid2D grid = getGrid();
        core::Sources sources(xMin, xMax, 2);
        core::Receivers receivers(xMin, xMax, 2);
        core::FrequenciesGroup frequencies(freq, 2000.0);

        forwardModels::ForwardModelMock forwardModel(grid, sources, receivers, frequencies);

        // Create measurement data
        const int lengthOfPData = forwardModel.getSource().count * forwardModel.getReceiver().count * forwardModel.getFreq().count;
        ;
        const std::vector<std::complex<double>> pData(lengthOfPData, 1.0);

        // Create a not existing direction calculator
        const std::string desiredStepSizeMethod = "FixedStepSize";
        const std::string desiredDirectionMethod = "";

        inversionMethods::StepAndDirectionReconstructorInput stepAndDirectionInput = getStepAndDirectionInput();

        Factory factory;
        EXPECT_THROW(factory.createStepAndDirectionReconstructor(stepAndDirectionInput, &forwardModel, desiredStepSizeMethod, desiredDirectionMethod, pData),
            std::invalid_argument);
    }
}   // namespace fwi
