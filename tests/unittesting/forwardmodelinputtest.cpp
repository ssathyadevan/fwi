#include <gtest/gtest.h>
#include "forwardModelInputCardReader.h"


//Generic Input Card test:
TEST(ForwardModelInputTest, ints){
    ASSERT_TRUE(true);
    std::string temp = "/../inputFiles/test/";
    forwardModelInputCardReader cardReader = forwardModelInputCardReader(temp);
    forwardModelInput input = cardReader.getInput();

    EXPECT_EQ(input.iter2.calcAlpha, 0);
    EXPECT_EQ(input.iter2.tolerance, 0.00005);
    EXPECT_EQ(input.iter2.n, 15);
}
