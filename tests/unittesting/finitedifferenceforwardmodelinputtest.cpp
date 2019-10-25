

#include <gtest/gtest.h>
#include <iostream>
#include "finiteDifferenceForwardModelInputCardReader.h"

//Generic Input Card test:
TEST(finiteDifferenceForwardModelInputTest, testCardToInputFunctionality){
    //std::string temp = "../../../tests/testCase/"; //jenkins path
    std::string temp = "../../parallelized-fwi/tests/testCase/"; //qt path

    finiteDifferenceForwardModelInputCardReader cardReader = finiteDifferenceForwardModelInputCardReader(temp);
    finiteDifferenceForwardModelInput input = cardReader.getInput();

    EXPECT_EQ(input.pmlWidthFactor.x, 10);
    EXPECT_EQ(input.pmlWidthFactor.z, 10);
    EXPECT_EQ(input.sourceParameter.r, 4);
    EXPECT_EQ(input.sourceParameter.beta, 15);
}
