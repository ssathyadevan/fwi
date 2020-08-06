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
        const std::array<double, 2> _xMin{0.0, 0.0};
        const std::array<double, 2> _xMax{2.0, 2.0};
        const std::array<int, 2> _nX{2, 4};
        const core::freqInfo _freq{1.0, 2.0, 2};
        const core::grid2D _grid{_xMin, _xMax, _nX};
        const core::Sources _sources{_xMin, _xMax, 2};
        const core::Receivers _receivers{_xMin, _xMax, 2};
        const core::FrequenciesGroup _frequencies{_freq, 2000.0};
        const int _lengthOfPData = _sources.count * _frequencies.count * _receivers.count;
        double _errorFunctionalScalingFactor = 1.0;

        const double _derivativeStepSize = 1.0;
        const bool _doRegression = false;
        const double _tolerance = 0.01;
        const double _startChi = 0.0;
        const int _maxIterations = 10;
        const double _initStepiSze = 1.0;
        const double _slope = -0.01;
        const inversionMethods::StepAndDirectionReconstructorInput _stepAndDirectionInput{
            {_tolerance, _startChi, _maxIterations}, {_initStepiSze, _slope}, {_derivativeStepSize}, _doRegression};
        NiceMock<forwardModels::ForwardModelMock> _forwardModel;
    };

    TEST_F(factoryTest, CreateStepAndDirectionReconstructor_MissingForwardModel_ExpectThrow)
    {
        // Create a fixed step size with conjugate gradient method
        const std::string desiredStepSizeMethod = "FixedStepSize";
        const std::string desiredDirectionMethod = "ConjugateGradientDirection";

        // Create measurement data
        const std::vector<std::complex<double>> pData(_lengthOfPData, 1.0);

        Factory factory;
        EXPECT_THROW(factory.createStepAndDirectionReconstructor(_stepAndDirectionInput, nullptr, desiredStepSizeMethod, desiredDirectionMethod, pData),
            std::invalid_argument);
    }

    TEST_F(factoryTest, CreateStepAndDirectionReconstructor_FixedStepSizeAndConjugateGradientDirectionSelected_ReconstructorIsNotNullPtr)
    {
        // Create a fixed step size with conjugate gradient method
        const std::string desiredStepSizeMethod = "FixedStepSize";
        const std::string desiredDirectionMethod = "ConjugateGradientDirection";

        // Create measurement data
        const std::vector<std::complex<double>> pData(_lengthOfPData, 1.0);

        Factory factory;
        inversionMethods::StepAndDirectionReconstructor *reconstructor;
        ON_CALL(_forwardModel, getGrid).WillByDefault(ReturnRef(_grid));
        reconstructor =
            factory.createStepAndDirectionReconstructor(_stepAndDirectionInput, &_forwardModel, desiredStepSizeMethod, desiredDirectionMethod, pData);
        EXPECT_FALSE(reconstructor == nullptr);
    }

    TEST_F(factoryTest, CreateStepAndDirectionReconstructor_NoDesiredStepSizeMethodSelected_ExpectThrow)
    {
        // Create a not existing step size calculator
        const std::string desiredStepSizeMethod = "";
        const std::string desiredDirectionMethod = "ConjugateGradientDirection";

        // Create measurement data
        const std::vector<std::complex<double>> pData(_lengthOfPData, 1.0);

        Factory factory;
        ON_CALL(_forwardModel, getGrid).WillByDefault(ReturnRef(_grid));
        EXPECT_THROW(factory.createStepAndDirectionReconstructor(_stepAndDirectionInput, &_forwardModel, desiredStepSizeMethod, desiredDirectionMethod, pData),
            std::invalid_argument);
    }

    TEST_F(factoryTest, CreateStepAndDirectionReconstructor_FixedStepSizeAndGradientDescentDirectionSelected_ReconstructorIsNotNullPtr)
    {
        // Create measurement data
        const std::vector<std::complex<double>> pData(_lengthOfPData, 1.0);

        // Create a fixed step with conjugate gradient descent method
        const std::string desiredStepSizeMethod = "FixedStepSize";
        const std::string desiredDirectionMethod = "GradientDescentDirection";

        Factory factory;
        inversionMethods::StepAndDirectionReconstructor *reconstructor;
        ON_CALL(_forwardModel, getGrid).WillByDefault(ReturnRef(_grid));
        reconstructor =
            factory.createStepAndDirectionReconstructor(_stepAndDirectionInput, &_forwardModel, desiredStepSizeMethod, desiredDirectionMethod, pData);
        EXPECT_FALSE(reconstructor == nullptr);
    }

    TEST_F(factoryTest, CreateStepAndDirectionReconstructor_NoDesiredDirectionMethodSelected_ExpectThrow)
    {
        // Create measurement data

        const std::vector<std::complex<double>> pData(_lengthOfPData, 1.0);

        // Create a not existing direction calculator
        const std::string desiredStepSizeMethod = "FixedStepSize";
        const std::string desiredDirectionMethod = "";

        Factory factory;
        ON_CALL(_forwardModel, getGrid).WillByDefault(ReturnRef(_grid));
        EXPECT_THROW(factory.createStepAndDirectionReconstructor(_stepAndDirectionInput, &_forwardModel, desiredStepSizeMethod, desiredDirectionMethod, pData),
            std::invalid_argument);
    }
}   // namespace fwi
