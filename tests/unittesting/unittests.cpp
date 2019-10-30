#include <gtest/gtest.h>
#include <iostream>

#include <grid2D.h>
#include <receivers.h>

/* TEMPLATE

TEST(CoreTest, )
{
    
}

*/

TEST(CoreTests, Grid2DTest)
{
    std::array<double, 2> x_min = {0.0, 0.0};
    std::array<double, 2> x_max = {2.0, 2.0};
    std::array<int, 2> n_x = {2,4};

    grid2D grid = grid2D(x_min, x_max, n_x);
    EXPECT_NEAR(grid.GetCellVolume(), 0.5, 0.01);
    EXPECT_EQ(grid.GetNumberOfGridPoints(), 8);
    EXPECT_NEAR(grid.GetDomainArea(), 4, 0.1);
}

TEST(CoreTest, ReceiverTest)
{
    std::array<double, 2> xMin{0, 0};
    std::array<double, 2> xMax{10, 0};
    int nRecv = 6;
    receivers receivers(xMin, xMax, nRecv);

    EXPECT_EQ(receivers.nRecv, nRecv);
    EXPECT_NEAR(receivers.xRecv[2][0], 4.0, 0.01);
    EXPECT_NEAR(receivers.xRecv[2][1], 0.0, 0.01);
}