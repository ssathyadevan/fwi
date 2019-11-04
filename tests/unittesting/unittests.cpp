#include <gtest/gtest.h>
#include <iostream>

#include <grid2D.h>
#include <receivers.h>
#include <sources.h>
#include <freq.h>
#include <frequenciesGroup.h>

#include <pressureFieldSerial.h>

/* TEMPLATE

TEST(CoreTest, )
{
    
}

*/

TEST(CoreTest, Grid2DTest)
{
    std::array<double, 2> x_min = {0.0, 0.0};
    std::array<double, 2> x_max = {2.0, 2.0};
    std::array<int, 2> n_x = {2,4};

    Grid2D grid = Grid2D(x_min, x_max, n_x);
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
    Receivers receivers(xMin, xMax, nRecv);

    EXPECT_EQ(receivers.nRecv, nRecv);
    EXPECT_NEAR(receivers.xRecv[5][0], 10.0, 0.01);
    EXPECT_NEAR(receivers.xRecv[5][1], 0.0, 0.01);
}

// Test source construction, assert that the last (5th) source is located at (10,0).
TEST(CoreTest, SourceTest)
{
    std::array<double, 2> xMin{0, 0};
    std::array<double, 2> xMax{10, 0};
    int nSrc = 6;
    Sources sources(xMin, xMax, nSrc);

    EXPECT_EQ(sources.nSrc, nSrc);
    EXPECT_NEAR(sources.xSrc[5][0], 10.0, 0.01);
    EXPECT_NEAR(sources.xSrc[5][1], 0.0, 0.01);
}

TEST(CoreTest, FrequenciesGroupTest)
{
    double min{10}, max{20};
    int n = 10;

    Freq freq{min, max, n};
    FrequenciesGroup freq_group{freq, 2000};

    EXPECT_NEAR(freq_group.k[9], 0.063, 0.01); // 2*pi*20/2000 ~ 0.063
}

TEST(CoreTest, PressureFieldSerialTest)
{
    std::array<double, 2> x_min_grid = {0, 0};
    std::array<double, 2> x_max_grid = {10, 10};
    std::array<int, 2> n_x_grid = {10, 10};
    Grid2D grid(x_min_grid, x_max_grid, n_x_grid);
    
    PressureFieldSerial pf_serial(grid);
    
    std::function<double(double, double)> f_field = [](double x, double z){return x+z;}; // Linear plane, x & z are centroids of grid cell.
    pf_serial.SetField(f_field);
    double* pf_data = pf_serial.GetDataPtr();

    EXPECT_EQ(pf_data[0], 1); // x + z = (0+(0+0.5)*1) + (0+(0+0.5)*1) = 0.5 + 0.5 = 1
    EXPECT_EQ(pf_data[99], 19);
}
