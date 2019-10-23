#include <gtest/gtest.h>
#include "integralForwardModelInputCardReader.h"


//Generic Input Card test:
TEST(integralForwardModelInputTest, ints){

    std::string temp = "../../../tests/testCase/"; //jenkins path
    //std::string temp = "../../../parallelized-fwi/tests/testCase/"; //qt path
    integralForwardModelInputCardReader cardReader = integralForwardModelInputCardReader(temp);
    integralForwardModelInput input = cardReader.getInput();

    EXPECT_EQ(input.iter2.calcAlpha, 0);
    EXPECT_EQ(input.iter2.tolerance, 0.00005);
    EXPECT_EQ(input.iter2.n, 100);
}
