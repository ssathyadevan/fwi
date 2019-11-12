#include <gtest/gtest.h>
#include "grid2D.h"
#include "pressureFieldSerial.h"

std::array<double, 2> x_min = {0.0, 0.0};
std::array<double, 2> x_max = {2.0, 2.0};
std::array<int, 2> n_x = {2,4};

Grid2D grid(x_min, x_max, n_x);

PressureFieldSerial pfs(grid);
std::function<double(double, double)> func = [](double x, double z){return x+z;}; // Linear plane, x & z are centroids of grid cell.

TEST(PressureFieldSerialTest, pfsNormTest)
{
    pfs.SetField(func);
    EXPECT_NEAR(pfs.Norm(), 6.04, 0.01);
}

TEST(PressureFieldSerialTest, pfsRelNormTest)
{
    pfs.SetField(func);
    EXPECT_NEAR(pfs.RelNorm(), 2.14, 0.01);
}

TEST(PressureFieldSerialTest, SquareTest)
{
    pfs.SetField(func);
    pfs.Square();
    double* ptr = pfs.GetDataPtr();
    EXPECT_NEAR(ptr[7], 10.5625, 0.001);
}

TEST(PressureFieldSerialTest, SqrtTest)
{
    pfs.SetField(func);
    pfs.Sqrt();
    double* ptr = pfs.GetDataPtr();
    EXPECT_NEAR(ptr[7], 1.80, 0.01);
}

TEST(PressureFieldSerialTest, ReciprocalTest)
{
    pfs.SetField(func);
    pfs.Reciprocal();
    double* ptr = pfs.GetDataPtr();
    EXPECT_NEAR(ptr[7], 0.30, 0.01);
}

TEST(PressureFieldSerialTest, InnerProductTest)
{
    pfs.SetField(func);
    EXPECT_NEAR(pfs.InnerProduct(pfs), 36.5, 0.01);
}