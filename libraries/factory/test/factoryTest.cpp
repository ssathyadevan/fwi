#include "factory.h"
#include "ForwardModelMock.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnRef;

namespace fwi
{
    class factoryTest : public ::testing::Test
    {
    public:
        const core::CostFunctionCalculator _costCalculator;
        std::array<double, 2> _xMin{0.0, 0.0};
        std::array<double, 2> _xMax{2.0, 2.0};
        std::array<int, 2> _nX{2, 4};
        double _errorFunctionalScalingFactor = 1.0;
        double _derivativeStepSize = 0.0;
        core::freqInfo _freq{1.0, 2.0, 2};
        core::grid2D _grid{_xMin, _xMax, _nX};
        core::Sources _sources{_xMin, _xMax, 2};
        core::Receivers _receivers{_xMin, _xMax, 2};
        core::FrequenciesGroup _frequencies{_freq, 2000.0};
        int _lengthOfPData = _sources.count * _frequencies.count * _receivers.count;
        NiceMock<forwardModels::ForwardModelMock> _forwardModel;
    };

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

    TEST_F(factoryTest, expectThrowMissingForwardModelTest)
    {
        // Create a fixed step size with conjugate gradient method
        const std::string desiredStepSizeMethod = "FixedStepSize";
        const std::string desiredDirectionMethod = "ConjugateGradientDirection";

        // Create measurement data
        const std::vector<std::complex<double>> pData(_lengthOfPData, 1.0);

        inversionMethods::StepAndDirectionReconstructorInput stepAndDirectionInput = getStepAndDirectionInput();

        Factory factory;
        EXPECT_THROW(factory.createStepAndDirectionReconstructor(stepAndDirectionInput, nullptr, desiredStepSizeMethod, desiredDirectionMethod, pData),
            std::invalid_argument);
    }

    TEST_F(factoryTest, createFixedStepSizeConjugateGradientMethodTest)
    {
        // Create a fixed step size with conjugate gradient method
        const std::string desiredStepSizeMethod = "FixedStepSize";
        const std::string desiredDirectionMethod = "ConjugateGradientDirection";

        // Create measurement data
        const std::vector<std::complex<double>> pData(_lengthOfPData, 1.0);

        inversionMethods::StepAndDirectionReconstructorInput stepAndDirectionInput = getStepAndDirectionInput();

        Factory factory;
        inversionMethods::StepAndDirectionReconstructor *reconstructor;
        ON_CALL(_forwardModel, getGrid).WillByDefault(ReturnRef(_grid));
        reconstructor =
            factory.createStepAndDirectionReconstructor(stepAndDirectionInput, &_forwardModel, desiredStepSizeMethod, desiredDirectionMethod, pData);
        EXPECT_FALSE(reconstructor == nullptr);
    }

    TEST_F(factoryTest, expectThrowStepSizeCalculatorTest)
    {
        // Create a not existing step size calculator
        const std::string desiredStepSizeMethod = "";
        const std::string desiredDirectionMethod = "ConjugateGradientDirection";

        // Create measurement data
        const std::vector<std::complex<double>> pData(_lengthOfPData, 1.0);

        inversionMethods::StepAndDirectionReconstructorInput stepAndDirectionInput = getStepAndDirectionInput();

        Factory factory;
        ON_CALL(_forwardModel, getGrid).WillByDefault(ReturnRef(_grid));
        EXPECT_THROW(factory.createStepAndDirectionReconstructor(stepAndDirectionInput, &_forwardModel, desiredStepSizeMethod, desiredDirectionMethod, pData),
            std::invalid_argument);
    }

    TEST_F(factoryTest, createFixedStepSizeGradientDescentMethodTest)
    {
        // Create measurement data
        const std::vector<std::complex<double>> pData(_lengthOfPData, 1.0);

        // Create a fixed step with conjugate gradient descent method
        const std::string desiredStepSizeMethod = "FixedStepSize";
        const std::string desiredDirectionMethod = "GradientDescentDirection";

        inversionMethods::StepAndDirectionReconstructorInput stepAndDirectionInput = getStepAndDirectionInput();

        Factory factory;
        inversionMethods::StepAndDirectionReconstructor *reconstructor;
        ON_CALL(_forwardModel, getGrid).WillByDefault(ReturnRef(_grid));
        reconstructor =
            factory.createStepAndDirectionReconstructor(stepAndDirectionInput, &_forwardModel, desiredStepSizeMethod, desiredDirectionMethod, pData);
        EXPECT_FALSE(reconstructor == nullptr);
    }

    TEST_F(factoryTest, expectThrowDirectionCalculatorTest)
    {
        // Create measurement data

        const std::vector<std::complex<double>> pData(_lengthOfPData, 1.0);

        // Create a not existing direction calculator
        const std::string desiredStepSizeMethod = "FixedStepSize";
        const std::string desiredDirectionMethod = "";

        inversionMethods::StepAndDirectionReconstructorInput stepAndDirectionInput = getStepAndDirectionInput();

        Factory factory;
        ON_CALL(_forwardModel, getGrid).WillByDefault(ReturnRef(_grid));
        EXPECT_THROW(factory.createStepAndDirectionReconstructor(stepAndDirectionInput, &_forwardModel, desiredStepSizeMethod, desiredDirectionMethod, pData),
            std::invalid_argument);
    }
}   // namespace fwi
