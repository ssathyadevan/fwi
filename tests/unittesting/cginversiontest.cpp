#include <gtest/gtest.h>
#include "conjugateGradientInversionInputCardReader.h"

//Generic Input Card test:
TEST(CGInversionInputTest, testCardToInputFunctionality)
{

    std::string temp = "../../../tests/testCase/"; //jenkins path
    //std::string temp = "../../../parallelized-fwi/tests/testCase/"; //qt path
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
