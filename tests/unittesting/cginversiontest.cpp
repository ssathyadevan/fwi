#include <gtest/gtest.h>
#include "conjugateGradientInversionInputCardReader.h"

#include <iostream>

//Generic Input Card test:
TEST(CGInversionInputTest, testCardToInputFunctionality){

    std::string temp = "../input/TestSet/";
    conjugateGradientInversionInputCardReader cardReader = conjugateGradientInversionInputCardReader(temp, "../output/", "testConjugateGradientInversionInput");
    conjugateGradientInput input = cardReader.getInput();

    std::cout << input.iteration1.tolerance << " " << input.dAmplification.start << " " << input.dAmplification.slope << " " << input.n_max << " " << input.iteration1.n << std::endl;

    EXPECT_EQ(input.iteration1.tolerance, 1e-08);
    EXPECT_EQ(input.dAmplification.start, 5e-5);
    EXPECT_EQ(input.dAmplification.slope, 10.0);
    EXPECT_EQ(input.n_max, 5);
    EXPECT_EQ(input.iteration1.n, 10);
    EXPECT_EQ(input.doReg, true);

}
