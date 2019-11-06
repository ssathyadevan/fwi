#include <gtest/gtest.h>
#include "../include/grid2D.h"

std::array<double, 2> x_min = {0.0, 0.0};
std::array<double, 2> x_max = {2.0, 2.0};
std::array<int, 2> n_x = {2,4};

Grid2D grid = Grid2D(x_min, x_max, n_x);

TEST(grid2DTest, GetCellVolumeTest)
{
    EXPECT_NEAR(grid.GetCellVolume(), 0.5, 0.01);
}