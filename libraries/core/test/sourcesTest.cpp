#include <gtest/gtest.h>
#include <iterator>
#include "sources.h"

// Grid2D getGrid()
// {
//     std::array<double, 2> x_min = {0.0, 0.0};
//     std::array<double, 2> x_max = {2.0, 2.0};
//     std::array<int, 2> n_x = {2,4};

//     Grid2D grid(x_min, x_max, n_x);
//     return grid;
// }

TEST(SourcesTest, ConstructorTest)
{
    std::array<double, 2> xMin = {0.0, 0.0};
    std::array<double, 2> xMax = {10.0, 10.0};
    Sources sources(xMin, xMax, 11);

    double dx = 1.0;
    double iteration_counter = 0.0;

    for (std::vector<std::array<double, 2>>::iterator iter = sources.xSrc.begin(); iter != sources.xSrc.end(); ++iter)
    {
        EXPECT_NEAR(iter->front(), iteration_counter*dx, 0.001);
        ++iteration_counter;
    }
}
