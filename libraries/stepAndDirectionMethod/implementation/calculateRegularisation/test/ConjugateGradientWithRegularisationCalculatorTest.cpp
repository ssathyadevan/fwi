#include "ConjugateGradientWithRegularisationCalculator.h"
#include "forwardmodelinterfacemock.h"
#include <gtest/gtest.h>
//#include <iomanip>
#include <iostream>

using std::cout;
using std::endl;

grid2D getGrid()
{
    std::array<double, 2> xMin = {0.0, 0.0};
    std::array<double, 2> xMax = {2.0, 2.0};
    std::array<int, 2> nX = {2, 5};
    grid2D grid(xMin, xMax, nX);

    return grid;
}

TEST(ConjugateGradientWithRegularisationCalculatorTest, calculateDirectionTest)
{
    double errorFunctionalScalingFactor = 0.05;

    grid2D grid = getGrid();
    std::array<double, 2> xMin = {0.0, 0.0};
    std::array<double, 2> xMax = {2.0, 2.0};
    freqInfo freq(0.0, 10.0, 5);
    sources src(xMin, xMax, 2);
    receivers recv(xMin, xMax, 2);
    frequenciesGroup frequencies(freq, 2000.0);

    ForwardModelInterfaceMock *forwardModel;
    forwardModel = new ForwardModelInterfaceMock(grid, src, recv, frequencies);

    double initialKappaTimesResidualValue = 1.0;
    forwardModel->setKappaTimesResidualValue(initialKappaTimesResidualValue);
    ConjugateGradientWithRegularisationParametersInput cgParametersInput;
    cgParametersInput._deltaAmplification._start = 1.0;
    cgParametersInput._deltaAmplification._slope = 0.0;
    cgParametersInput._nRegularisationIterations = 0;   // not using regularisation
    cgParametersInput._tolerance = 0.001;

    int nTotal = forwardModel->getSrc().nSrc * forwardModel->getFreq().nFreq * forwardModel->getRecv().nRecv;
    const double pDataValue = 1.0;
    std::vector<std::complex<double>> pData(nTotal, pDataValue);

    ConjugateGradientWithRegularisationCalculator cGWRCTest(errorFunctionalScalingFactor, forwardModel, cgParametersInput, pData);

    DirectionCalculator *directionCalculator;
    directionCalculator = &cGWRCTest;

    dataGrid2D chiEstimateCurrent(getGrid());
    // chiEstimateCurrent.data[] =0
    std::vector<std::complex<double>> residualVector =
        forwardModel->calculateResidual(chiEstimateCurrent, pData);   // chiEstimateCurrent is all 0s and unused, pData is all 1s
    dataGrid2D ignoreThis(getGrid());
    dataGrid2D const *directionCurrent;
    directionCurrent = &directionCalculator->calculateDirection(ignoreThis, residualVector);

    dataGrid2D directionTest(getGrid());
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

    int nextIteration = 1;
    cGWRCTest.updateVariables(chiEstimateCurrent, ignoreThis, nextIteration);

    // updating KappaTimesResidual to simulate a change in the complexDataGrid2D** _Kappa in ForwardInterfaceMock
    double kappaTimesResidualMultiplier = 3;
    forwardModel->setKappaTimesResidualValue(kappaTimesResidualMultiplier * initialKappaTimesResidualValue);

    residualVector = forwardModel->calculateResidual(chiEstimateCurrent, pData);
    // second direction computed
    directionCurrent = &directionCalculator->calculateDirection(ignoreThis, residualVector);

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

////////////////////////////////////////////////////////////////
// The following test will check if a whole round of the regularisation part, invoked by ->calculateStepSize(), works. Since there ar no existing tests for
// ConjugateGradientInversion (and as of 15-05-'20 there is also a bug), the only way to test our method is to fix such bug in the old design, use that as
// benchmark and compare our new results with that. The commented code is slightly obsolete, but is is possible to use the above test body as starting point.
////////////////////////////////////////////////////////////////

// TEST(ConjugateGradientWithRegularisationCalculatorTest, calculateStepSizeTest)
//{
//    double errorFunctionalScalingFactor = 1.0;

//    grid2D grid = getGrid();
//    std::array<double, 2> xMin = {0.0, 0.0};
//    std::array<double, 2> xMax = {2.0, 2.0};
//    freqInfo freq(0.0, 10.0, 5);
//    sources sources(xMin, xMax, 2);
//    receivers receivers(xMin, xMax, 2);
//    frequenciesGroup frequencies(freq, 2000.0);

//    finiteDifferenceForwardModelInput fmInput;
//    fmInput.sourceParameter.r = 4;
//    fmInput.sourceParameter.beta = 6.31;

//    forwardModelInterface *forwardModel;
//    forwardModel = new finiteDifferenceForwardModel(grid, sources, receivers, frequencies, fmInput);

//    ConjugateGradientWithRegularisationParametersInput cgParametersInput;
//    cgParametersInput._deltaAmplification._start = 1.0;
//    cgParametersInput._deltaAmplification._slope = 0.0;
//    cgParametersInput._nRegularisationIterations = 0;   // not using regularisation
//    cgParametersInput._tolerance = 0.001;

//    int nTotal = forwardModel->getSrc().nSrc * forwardModel->getFreq().nFreq * forwardModel->getRecv().nRecv;
//    const double pDataValue = 1.0;
//    std::vector<std::complex<double>> pData(nTotal, pDataValue);

//    ConjugateGradientWithRegularisationCalculator cGWRCTest(errorFunctionalScalingFactor, forwardModel, cgParametersInput, pData);

//    //   DirectionCalculator *directionCalculator;
//    // directionCalculator = *cGWRCTest;
//    StepSizeCalculator *stepSizeCalculator;
//    stepSizeCalculator = &cGWRCTest;

//    // stepSizeCalculator->updateVariables(chiEstimateCurrent, derivativeCurrent, 1);

//    //    double stepSizeZero = 0.5;
//    //    double stepSizeCalculate = stepSizeCalculator->calculateStepSize();

//    // ASSERT_DOUBLE_EQ(stepSizeCalculate, stepSizeZero);

//    delete forwardModel;
//}
