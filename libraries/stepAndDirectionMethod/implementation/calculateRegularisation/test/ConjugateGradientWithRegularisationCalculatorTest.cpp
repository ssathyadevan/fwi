#include "ConjugateGradientWithRegularisationCalculator.h"
#include "finiteDifferenceForwardModel.h"
#include <gtest/gtest.h>
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
    /*   double errorFunctionalScalingFactor = 1.0;

       grid2D grid = getGrid();
       std::array<double, 2> xMin = {0.0, 0.0};
       std::array<double, 2> xMax = {2.0, 2.0};
       freqInfo freq(0.0, 10.0, 5);
       sources src(xMin, xMax, 2);
       receivers recv(xMin, xMax, 2);
       frequenciesGroup frequencies(freq, 2000.0);

       finiteDifferenceForwardModelInput fmInput;
       fmInput.sourceParameter.r = 4;
       fmInput.sourceParameter.beta = 6.31;

       forwardModelInterface *forwardModel;
       forwardModel = new finiteDifferenceForwardModel(grid, src, recv, frequencies, fmInput);

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
       std::vector<std::complex<double>> residualVector =
           forwardModel->calculateResidual(chiEstimateCurrent, pData);   // chiEstimateCurrent is all 0s and unused, pData is all 1s

       dataGrid2D const *directionCurrent;
       directionCurrent = &directionCalculator->calculateDirection(chiEstimateCurrent, residualVector);
       (void)directionCurrent;
       dataGrid2D directionTest(getGrid());
       directionTest = 1.0;

       // for the moment I just want to see what values I am getting out, later I will create a boolean to compare the two vectors

       //    std::vector<double> directionCurrentData = directionCurrent->getData();
       //    std::vector<double> directionTestData = directionTest.getData();

       //    for(int i = 0; i < directionTest.getNumberOfGridPoints(); ++i)
       //    {
       //        cout << "directionCurrentData = " << directionCurrentData[i] << ", directionTestData = " << directionTestData[i] << endl;
       //    }

       //   ASSERT_EQ(directionCurrentData, directionTestData);

       delete forwardModel; */
}

////////////////////////////////////////////////////////////////
// The following test should check a whole round of the regularisation process. Making a benchmark for this part is highly time consuming and potentially
// useless, since there are parts of the code that are not documented/explained, so our only benchmark would be the results of ConjugateGradientInversion, which
// also has no test.
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
