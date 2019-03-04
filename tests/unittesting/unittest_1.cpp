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



//int main(int argc, char **argv)
//{
//    ::testing::InitGoogleTest(&argc, argv);
//    int ret = RUN_ALL_TESTS();
//    return ret;
//}

