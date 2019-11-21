#include <gtest/gtest.h>
#include <iterator>
#include "sources.h"

TEST(SourcesTest, ConstructorTest)
{
    std::array<double, 2> xMin = {0.0, 0.0};
    std::array<double, 2> xMax = {10.0, 10.0};
    Sources sources(xMin, xMax, 11);

    double dx = 1.0;
    double iteration_counter = 0.0;

    for (std::vector<std::array<double, 2>>::iterator iter = sources.xSrc.begin(); iter != sources.xSrc.end(); ++iter)
    {
        EXPECT_NEAR(iter->front(), iteration_counter * dx, 0.001);
        EXPECT_NEAR(iter->back(), iteration_counter * dx, 0.001);
        ++iteration_counter;
    }
}
