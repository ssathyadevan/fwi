#include "ConjugateGradientWithRegularisationCalculator.h"
#include "ForwardModelMock.h"
#include <gtest/gtest.h>
#include <iostream>

using std::cout;
using std::endl;

namespace fwi
{
    namespace inversionMethods
    {
        core::grid2D getGrid()
        {
            std::array<double, 2> xMin = {0.0, 0.0};
            std::array<double, 2> xMax = {2.0, 2.0};
            std::array<int, 2> nX = {2, 5};
            core::grid2D grid(xMin, xMax, nX);

            return grid;
        }

        TEST(ConjugateGradientWithRegularisationCalculatorTest, calculateDirectionTest)
        {
            double errorFunctionalScalingFactor = 0.05;

            core::CostFunctionCalculator costCalculator;
            std::array<double, 2> xMin = {0.0, 0.0};
            std::array<double, 2> xMax = {2.0, 2.0};
            std::array<int, 2> nX = {2, 5};
            core::grid2D grid(xMin, xMax, nX);
            core::freqInfo freq(0.0, 10.0, 5);
            core::Sources source(xMin, xMax, 2);
            core::Receivers receiver(xMin, xMax, 2);
            core::FrequenciesGroup frequencies(freq, 2000.0);

            forwardModels::ForwardModelMock *forwardModel;
            forwardModel = new forwardModels::ForwardModelMock(grid, source, receiver, frequencies);

            double initialKappaTimesResidualValue = 1.0;
            forwardModel->setKappaTimesResidualValue(initialKappaTimesResidualValue);
            ConjugateGradientWithRegularisationParametersInput cgParametersInput;
            cgParametersInput._deltaAmplification._start = 1.0;
            cgParametersInput._deltaAmplification._slope = 0.0;
            cgParametersInput._nIterations = 0;
            cgParametersInput._tolerance = 0.001;

            int nTotal = forwardModel->getSource().count * forwardModel->getFreq().count * forwardModel->getReceiver().count;
            const double pDataValue = 1.0;
            std::vector<std::complex<double>> pData(nTotal, pDataValue);

            ConjugateGradientWithRegularisationCalculator cGWRCTest(errorFunctionalScalingFactor, costCalculator, forwardModel, cgParametersInput, pData);

            DirectionCalculator *directionCalculator;
            directionCalculator = &cGWRCTest;

            core::dataGrid2D chiEstimateCurrent(getGrid());
            // chiEstimateCurrent.data[] =0
            std::vector<std::complex<double>> residualVector =
                forwardModel->calculateResidual(chiEstimateCurrent, pData);   // chiEstimateCurrent is all 0s and unused, pData is all 1s
            core::dataGrid2D mockDataGrid(getGrid());
            core::dataGrid2D const *directionCurrent;
            directionCurrent = &directionCalculator->calculateDirection(mockDataGrid, residualVector);

            core::dataGrid2D directionTest(getGrid());
            directionTest = errorFunctionalScalingFactor;
            int nGridPoints = directionTest.getNumberOfGridPoints();

            std::vector<double> directionCurrentData = directionCurrent->getData();
            std::vector<double> directionTestData1 = directionTest.getData();

            // first iteration
            EXPECT_TRUE(directionCurrentData == directionTestData1);

            // second iteration
            double step = 1.0;
            for(int i = 0; i < nGridPoints; ++i)
            {
                chiEstimateCurrent.addValueAtIndex(step * directionCurrentData[i], i);
            }
            std::vector<std::complex<double>> mockVector;
            int nextIteration = 1;
            cGWRCTest.updateVariables(chiEstimateCurrent, mockDataGrid, nextIteration, mockVector, mockVector);

            // updating KappaTimesResidual to simulate a change in the core::complexDataGrid2D** _Kappa in ForwardInterfaceMock
            double kappaTimesResidualMultiplier = 3;
            forwardModel->setKappaTimesResidualValue(kappaTimesResidualMultiplier * initialKappaTimesResidualValue);

            residualVector = forwardModel->calculateResidual(chiEstimateCurrent, pData);
            // second direction computed
            directionCurrent = &directionCalculator->calculateDirection(mockDataGrid, residualVector);

            directionCurrentData = directionCurrent->getData();
            std::vector<double> directionTestData2 = directionTest.getData();

            // updating directionTestData2 and computing square error
            double squareDiffNorm = 0.0;
            for(int i = 0; i < nGridPoints; ++i)
            {
                directionTestData2[i] *= kappaTimesResidualMultiplier * kappaTimesResidualMultiplier;

                squareDiffNorm += std::pow(directionCurrentData[i] - directionTestData2[i], 2);
            }

            EXPECT_NEAR(squareDiffNorm, 0, 1e-10);

            delete forwardModel;
        }
    }   // namespace inversionMethods
}   // namespace fwi
