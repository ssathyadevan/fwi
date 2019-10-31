#include <gtest/gtest.h>
#include <iostream>

#include <grid2D.h>
#include <receivers.h>
#include <sources.h>
#include <freq.h>
#include <frequenciesGroup.h>

#include <iter2.h>
#include <integralForwardModel.h>
#include <integralForwardModelInput.h>
#include <forwardModelInterface.h>
#include <greensSerial.h>

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

// Test receiver construction, assert that the last (5th) receiver is located at (10,0).
TEST(CoreTest, ReceiverTest)
{
    std::array<double, 2> xMin{0, 0};
    std::array<double, 2> xMax{10, 0};
    int nRecv = 6;
    receivers receivers(xMin, xMax, nRecv);

    EXPECT_EQ(receivers.nRecv, nRecv);
    EXPECT_NEAR(receivers.xRecv[5][0], 10.0, 0.01);
    EXPECT_NEAR(receivers.xRecv[5][1], 0.0, 0.01);
}

// Test source construction, assert that the last (5th) source is located at (10,0).
TEST(CoreTest, SourceTest)
{
    std::array<double, 2> xMin{0, 0};
    std::array<double, 2> xMax{10, 0};.
    int nSrc = 6;
    sources sources(xMin, xMax, nSrc);

    EXPECT_EQ(sources.nSrc, nSrc);
    EXPECT_NEAR(sources.xSrc[5][0], 10.0, 0.01);
    EXPECT_NEAR(sources.xSrc[5][1], 0.0, 0.01);
}

TEST(CoreTest, FrequenciesGroupTest)
{
    double min{10}, max{20};
    int n = 10;

    Freq freq{min, max, n};
    frequenciesGroup freq_group{freq, 2000};

    EXPECT_NEAR(freq_group.k[9], 0.063, 0.01); // 2*pi*20/2000 ~ 0.063
}

/*TEST(CoreTest, IntegralForwardModelTest)
{
    // test configuration:
    //  ssssssrrrrr
    //  -----------
    //  |  10x10  |
    //  -----------   
    //
    double x_min_grid[2] = {0, 0};
    double x_max_grid[2] = {10, 10};
    int n_x_grid[2] = {10, 10};
    grid2D grid(x_min_grid, x_max_grid, n_x_grid);
    
    double x_min_src[2] = {0, 0};
    double x_max_src[2] = {5, 0};
    int n_src[2] = {6, 1};
    sources sources(x_min_src, x_max_src, n_src);

    double x_min_recv[2] = {6, 0};
    double x_max_recv[2] = {10, 0};
    int n_recv[2] = {5, 0};
    receivers receivers(x_min_recv, x_max_recv, n_recv);

    double f_min{10}, f_max{20};
    int n_freqs = 10;
    Freq freq{min, max, n};
    frequenciesGroup freq_group{freq, 2000};

    Iter2 iter{15, 5.0e-5, false};
    integralForwardModelInput ifm_input{iter};

    IntegralForwardModel if_model{grid, sources, receivers, freq_group, ifm_input};
    if_model.calculateKappa();
    if_model.calculateResidual(pressureFieldSerial chi_est(grid), )
}*/
