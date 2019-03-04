#include <gtest/gtest.h>
#include "conjugateGradientInversionInputCardReader.h"


//Generic Input Card test:
TEST(CGInversionInputTest, testCardToInputFunctionality){

    std::string temp = "../../../inputFiles/test/"; // this is a path specific for jenkins
    //std::string temp = "../../../parallelized-fwi/inputFiles/test/"; // this is a path when running tests in qt
    conjugateGradientInversionInputCardReader cardReader = conjugateGradientInversionInputCardReader(temp);
    conjugateGradientInput input = cardReader.getInput();

    //ASSERT_TRUE(true);

    EXPECT_EQ(input.iteration1.tolerance, 1e-08);
    EXPECT_EQ(input.dAmplification.start, 100.0);
    EXPECT_EQ(input.dAmplification.slope, 10.0);
    EXPECT_EQ(input.n_max, 5);
    EXPECT_EQ(input.iteration1.n, 10);
    EXPECT_EQ(input.doReg, true);
}

