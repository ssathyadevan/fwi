#include <gtest/gtest.h>
#include <utilityFunctions.h>
#include "genericInputCardReader.h"




TEST (HelperFunctionTest, ints){
    ASSERT_EQ(5, dist(3,4));

}

TEST (HelperFunctionTest, doubles){

    EXPECT_EQ(5., dist(3.,4.));

}

TEST (HelperFunctionTest, doubles2){

    EXPECT_EQ(5., dist(3.,4.));
}

//Generic Input Card test:
TEST(GenericInputTest, ints){

    std::string temp = "../input/";
    genericInputCardReader cardReader = genericInputCardReader(temp,"/output/","testGenericInput");
    genericInput input = cardReader.getInput();
    EXPECT_EQ(input.ngrid[0],64);
    EXPECT_EQ(input.ngrid[1],32);
    EXPECT_EQ(input.freq.nTotal, 15);
    EXPECT_EQ(input.nSourcesReceivers.rec, 17);
    EXPECT_EQ(input.nSourcesReceivers.src, 17);

}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    return ret;
}

