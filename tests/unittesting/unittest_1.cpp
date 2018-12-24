#include <gtest/gtest.h>
#include <utilityFunctions.h>


TEST (HelperFunctionTest, ints){
    ASSERT_EQ(5, dist(3,4));
    ASSERT_EQ(10 , dist(6,8));

}

TEST (HelperFunctionTest, DISABLED_doubles){
    EXPECT_EQ(19.2094, dist(12,15));
    EXPECT_EQ(6., dist(4.,4.));

}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    return ret;
}

