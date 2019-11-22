#include <gtest/gtest.h>
#include <iterator>
#include "receivers.h"

TEST(ReceiversTest, ConstructorTest)
{
    std::array<double, 2> xMin = {0.0, 0.0};
    std::array<double, 2> xMax = {10.0, 10.0};
    Receivers receivers(xMin, xMax, 11);

    double dx = 1.0;
    double iteration_counter = 0.0;

    for (std::vector<std::array<double, 2>>::iterator iter = receivers.xRecv.begin(); iter != receivers.xRecv.end(); ++iter)
    {
        EXPECT_NEAR(iter->front(), iteration_counter*dx, 0.001);
        EXPECT_NEAR(iter->back(), iteration_counter*dx, 0.001);
        ++iteration_counter;
    }
}
