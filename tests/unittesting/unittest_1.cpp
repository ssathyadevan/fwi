#include <gtest/gtest.h>
#include <utilityFunctions.h>


TEST (HelperFunctionTest, ints){
    ASSERT_EQ(5, dist(3,4));
    ASSERT_EQ(10 , dist(6,8));

}

TEST (HelperFunctionTest, doubles){

    EXPECT_EQ(5., dist(4.,3.));

}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    return ret;
}

