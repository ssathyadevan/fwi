#include <gtest/gtest.h>
#include "grid2D.h"

std::array<double, 2> x_min = {0.0, 0.0};
std::array<double, 2> x_max = {2.0, 2.0};
std::array<int, 2> n_x = {2,4};

Grid2D grid = Grid2D(x_min, x_max, n_x);

TEST(grid2DTest, getGridDimensionsTest)
{
    EXPECT_EQ(grid.GetGridDimensions(), n_x);
}

TEST(grid2DTest, GetCellDimensionsTest)
{
    std::array<double, 2> a = grid.GetCellDimensions();
    
    std::array<double, 2> b;
    b[0] = 2.0/2.0;
    b[1] = 2.0/4.0;
    
    for(int i=0; i<2; i++)
    {
        EXPECT_NEAR(a[i], b[i], 0.01);
    }
}

TEST(grid2DTest, GetGridStartTest)
{
    EXPECT_EQ(grid.GetGridStart(), x_min);
}

TEST(grid2DTest, GetGridEndTest)
{
    EXPECT_EQ(grid.GetGridEnd(), x_max);
}

TEST(grid2DTest, GetNumverOfGridPointsTest)
{
    EXPECT_EQ(grid.GetNumberOfGridPoints(), 8);
}

TEST(grid2DTest, GetCellVolumeTest)
{
    EXPECT_NEAR(grid.GetCellVolume(), 0.5, 0.01);
}

TEST(grid2DTest, getDomainAreaTest)
{
    EXPECT_NEAR(grid.GetDomainArea(), (x_max[0] - x_min[0]) * (x_max[1] - x_min[1]), 0.01);
}