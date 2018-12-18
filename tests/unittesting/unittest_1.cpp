#include <gtest/gtest.h>
#include <utilityFunctions.h>


TEST (HelperFunctionTest, TwoValues){
    ASSERT_EQ(19.2094, dist(12,15));
    ASSERT_EQ(6., dist(4.,4.));

}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    return ret;
}

