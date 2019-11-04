#include <gtest/gtest.h>

// Input Card Readers
#include "conjugateGradientInversionInputCardReader.h"
#include "genericInputCardReader.h"
#include "finiteDifferenceForwardModelInputCardReader.h"
#include "randomInversionInputCardReader.h"
#include "gradientDescentInversionInputCardReader.h"
#include "integralForwardModelInputCardReader.h"
#include "residualTracker.h"

//Conjugate Gradient Input Card Reader Test:
TEST(InputTest, conjugateGradientTest)
{
    //std::string temp = "../../../tests/testCase/"; //jenkins path
    std::string temp = "../../parallelized-fwi/tests/testCase"; //qt path
    conjugateGradientInversionInputCardReader cardReader = conjugateGradientInversionInputCardReader(temp);
    conjugateGradientInversionInput input = cardReader.getInput();

    //ASSERT_TRUE(true);

    EXPECT_EQ(input.iteration1.tolerance, 1e-08);
    EXPECT_EQ(input.dAmplification.start, 100.0);
    EXPECT_EQ(input.dAmplification.slope, 10.0);
    EXPECT_EQ(input.n_max, 5);
    EXPECT_EQ(input.iteration1.n, 10);
    EXPECT_EQ(input.doReg, true);
}

/*
//Generic Input Card Reader Test:
TEST(InputTest, genericTest)
{
    //std::string temp = "../../../tests/testCase/"; //jenkins path
    std::string temp = "../../../parallelized-fwi/tests/testCase"; //qt path

    genericInputCardReader cardReader = genericInputCardReader(temp);
    genericInput input = cardReader.getInput();

    EXPECT_EQ(input.ngrid[0], 64); // Fails, number expected, null returned.
    //EXPECT_EQ(input.ngrid[1], 32);
    //EXPECT_EQ(input.freq.nTotal, 15);
    //EXPECT_EQ(input.nSourcesReceivers.rec, 17);
    //EXPECT_EQ(input.nSourcesReceivers.src, 17);
}
*/

//Finite Difference Forward Model Input Card Reader Test:
TEST(InputTest, finiteDifferenceForwardModelTest){
    //std::string temp = "../../../tests/testCase/"; //jenkins path
    std::string temp = "../../parallelized-fwi/tests/testCase"; //qt path

    finiteDifferenceForwardModelInputCardReader cardReader = finiteDifferenceForwardModelInputCardReader(temp);
    finiteDifferenceForwardModelInput input = cardReader.getInput();

    EXPECT_EQ(input.pmlWidthFactor.x, 10);
    EXPECT_EQ(input.pmlWidthFactor.z, 10);
    EXPECT_EQ(input.sourceParameter.r, 4);
    EXPECT_EQ(input.sourceParameter.beta, 15);
}

//Random Inversion Input Card Reader Test:
TEST(InputTest, randomInversionTest){
    //std::string temp = "../../../tests/testCase/"; //jenkins path
    std::string temp = "../../parallelized-fwi/tests/testCase"; //qt path

    randomInversionInputCardReader cardReader = randomInversionInputCardReader(temp);
    randomInversionInput input = cardReader.getInput();

    EXPECT_EQ(input.nMaxInner, 10);
    EXPECT_EQ(input.nMaxOuter, 10);
    EXPECT_EQ(input.toleranceOuter, 5.0e-5);
}

//Gradient Descent Inversion Input Card Reader Test:
TEST(InputTest, gradientDescentInversionTest){
    //std::string temp = "../../../tests/testCase/"; //jenkins path
    std::string temp = "../../parallelized-fwi/tests/testCase"; //qt path

    gradientDescentInversionInputCardReader cardReader = gradientDescentInversionInputCardReader(temp);
    gradientDescentInversionInput input = cardReader.getInput();

    EXPECT_EQ(input.gamma0, 0.1);
    EXPECT_EQ(input.x0, 0.5);
    EXPECT_EQ(input.iter, 5);
}

//Integral Forward Model Input Card Reader Test:
TEST(InputTest, integralForwardModelTest){
    //std::string temp = "../../../tests/testCase/"; //jenkins path
    std::string temp = "../../parallelized-fwi/tests/testCase"; //qt path

    integralForwardModelInputCardReader cardReader = integralForwardModelInputCardReader(temp);
    integralForwardModelInput input = cardReader.getInput();

    EXPECT_EQ(input.iter2.n, 100);
    EXPECT_EQ(input.iter2.tolerance, 5.0e-5);
    EXPECT_EQ(input.iter2.calcAlpha, false);
}

TEST(InputTest, residualTrackerConvergenceTest) {
    ResidualTracker residualTracker;

    std::vector<double> convergingSequence = {1.0, 0.7, 0.4, 0.9, 0.6, 0.3, 0.8, 0.5, 0.2};

    for (double& residual : convergingSequence) {
        residualTracker.determineIfDiverging(residual);
    }
    EXPECT_EQ(residualTracker.isDiverging(), false);
}

TEST(InputTest, residualTrackerDivergenceTest) {
    ResidualTracker residualTracker;

    std::vector<double> divergingSequence = {0.8, 0.5, 0.2, 0.9, 0.6, 0.3, 1.0, 0.7, 0.4};

    for (double& residual : divergingSequence) {
        residualTracker.determineIfDiverging(residual);
    }
    EXPECT_EQ(residualTracker.isDiverging(), true);
}
