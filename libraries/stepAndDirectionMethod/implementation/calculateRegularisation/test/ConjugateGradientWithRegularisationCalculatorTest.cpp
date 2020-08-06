#include "ConjugateGradientWithRegularisationCalculator.h"
#include "ForwardModelMock.h"
#include <iostream>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnRef;

using fwi::core::operator-;

namespace fwi
{
    namespace inversionMethods
    {
        class ConjugateGradientWithRegularisationCalculatorTest : public ::testing::Test
        {
        protected:
            const std::array<double, 2> _xMin{0.0, 0.0};
            const std::array<double, 2> _xMax{2.0, 2.0};
            const std::array<int, 2> _nX{2, 5};
            const double _errorFunctionalScalingFactor = 0.05;
            const core::grid2D _grid{_xMin, _xMax, _nX};
            const core::Sources _source{_xMin, _xMax, 2};
            const core::Receivers _receiver{_xMin, _xMax, 2};
            const core::freqInfo _freq{0.0, 10.0, 5};
            const core::FrequenciesGroup _frequencies{_freq, 2000.0};
            const core::CostFunctionCalculator _costCalculator;
            NiceMock<forwardModels::ForwardModelMock> _forwardModel;
        };

        TEST_F(ConjugateGradientWithRegularisationCalculatorTest, calculateDirection_TwoIterations_Convergance)
        {
            double initialKappaTimesResidualValue = 1.0;

            ConjugateGradientWithRegularisationParametersInput cgParametersInput;
            cgParametersInput._deltaAmplification._start = 1.0;
            cgParametersInput._deltaAmplification._slope = 0.0;
            cgParametersInput._nIterations = 0;
            cgParametersInput._tolerance = 0.001;

            const double pDataValue = 1.0;
            std::vector<std::complex<double>> pData(_source.count * _frequencies.count * _receiver.count, pDataValue);

            ON_CALL(_forwardModel, getGrid).WillByDefault(ReturnRef(_grid));
            ON_CALL(_forwardModel, getSource).WillByDefault(ReturnRef(_source));
            ON_CALL(_forwardModel, getReceiver).WillByDefault(ReturnRef(_receiver));
            ON_CALL(_forwardModel, getFreq).WillByDefault(ReturnRef(_frequencies));

            ConjugateGradientWithRegularisationCalculator cGWRCTest(_errorFunctionalScalingFactor, _costCalculator, &_forwardModel, cgParametersInput, pData);

            DirectionCalculator *directionCalculator;
            directionCalculator = &cGWRCTest;

            core::dataGrid2D chiEstimateCurrent(_grid);
            ON_CALL(_forwardModel, calculatePressureField)
                .WillByDefault(Return(std::vector<std::complex<double>>(pData.size(), (chiEstimateCurrent.getData())[0])));
            std::vector<std::complex<double>> residualVector = pData - _forwardModel.calculatePressureField(chiEstimateCurrent);
            core::dataGrid2D mockDataGrid(_grid);
            core::dataGrid2D const *directionCurrent;
            ON_CALL(_forwardModel, getUpdateDirectionInformation(_, _)).WillByDefault(testing::SetArgReferee<1>(initialKappaTimesResidualValue));
            directionCurrent = &directionCalculator->calculateDirection(mockDataGrid, residualVector);

            core::dataGrid2D directionTest(_grid);
            directionTest = _errorFunctionalScalingFactor;

            std::vector<double> directionCurrentData = directionCurrent->getData();
            std::vector<double> directionTestData1 = directionTest.getData();

            // first iteration
            EXPECT_TRUE(directionCurrentData == directionTestData1);

            // second iteration
            const double step = 1.0;
            for(size_t i = 0; i < directionCurrentData.size(); ++i)
            {
                chiEstimateCurrent.addValueAtIndex(step * directionCurrentData[i], i);
            }
            std::vector<std::complex<double>> mockVector;
            int nextIteration = 1;
            cGWRCTest.updateVariables(chiEstimateCurrent, mockDataGrid, nextIteration, mockVector, mockVector);

            // updating KappaTimesResidual to simulate a change in the core::complexDataGrid2D** _Kappa in ForwardInterfaceMock
            double kappaTimesResidualMultiplier = 3;

            residualVector = pData - _forwardModel.calculatePressureField(chiEstimateCurrent);
            // second direction computed

            ON_CALL(_forwardModel, getUpdateDirectionInformation(_, _))
                .WillByDefault(testing::SetArgReferee<1>(kappaTimesResidualMultiplier * initialKappaTimesResidualValue));
            directionCurrent = &directionCalculator->calculateDirection(mockDataGrid, residualVector);

            directionCurrentData = directionCurrent->getData();
            std::vector<double> directionTestData2 = directionTest.getData();

            // updating directionTestData2 and computing square error
            double squareDiffNorm = 0.0;
            for(size_t i = 0; i < directionCurrentData.size(); ++i)
            {
                directionTestData2[i] *= kappaTimesResidualMultiplier * kappaTimesResidualMultiplier;

                squareDiffNorm += std::pow(directionCurrentData[i] - directionTestData2[i], 2);
            }

            EXPECT_NEAR(squareDiffNorm, 0, 1e-10);
        }
    }   // namespace inversionMethods
}   // namespace fwi
