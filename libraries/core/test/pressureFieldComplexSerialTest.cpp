#include <gtest/gtest.h>
#include "pressureFieldComplexSerial.h"
#include <iostream>

Grid2D getGrid()
{
    std::array<double, 2> x_min = {0.0, 0.0};
    std::array<double, 2> x_max = {2.0, 2.0};
    std::array<int, 2> n_x = {2,4};

    Grid2D grid(x_min, x_max, n_x);
    return grid;
}

PressureFieldComplexSerial getPFCS()
{
    Grid2D grid = getGrid();
    PressureFieldComplexSerial pfcs(grid);
    std::function<std::complex<double>(double, double)> func = [](double a, double b){return std::complex<double>(a, b);}; // y = a + bi
    pfcs.SetField(func);

    return pfcs;
}

TEST(pressureFieldComplexSerialTest, squareTest)
{
    PressureFieldComplexSerial pfcs(getGrid());
    pfcs = getPFCS();
    /* Reason for initiating this way:
    *       When doing: PressureFieldComplexSerial pfcs = getPFCS();
    *       The copy-constructor is called, which uses memcpy().
    *       In order to do succeed, both sides of the '=' sign must have equal grids already.
    *       This is guaranteed by initialising an empty PF of 'size' [grid].
    */
    
    pfcs.Square();

    std::complex<double>* ptr = pfcs.GetDataPtr();
    EXPECT_NEAR(std::real(ptr[0]), 0.1875, 0.001);
    EXPECT_NEAR(std::imag(ptr[0]), 0.25, 0.001);
    EXPECT_NEAR(std::real(ptr[7]), -0.8125, 0.001);
    EXPECT_NEAR(std::imag(ptr[7]), 5.25, 0.001);
}

TEST(pressureFieldComplexSerialTest, setFieldTest)
{
    PressureFieldComplexSerial pfcs(getGrid()); // Empty PFCS of size grid
    std::function<std::complex<double>(double, double)> func = [](double a, double b){return std::complex<double>(a, b);};
    pfcs.SetField(func);
    
    std::complex<double>* ptr = pfcs.GetDataPtr();
    EXPECT_EQ(sizeof(*ptr), 8*2);
    EXPECT_EQ(std::real(ptr[0]), 0.5);
    EXPECT_EQ(std::imag(ptr[0]), 0.25);
    EXPECT_EQ(std::real(ptr[7]), 1.5);
    EXPECT_EQ(std::imag(ptr[7]), 1.75);
}

TEST(pressureFieldComplexSerialTest, reciprocalTest)
{
    PressureFieldComplexSerial pfcs(getGrid());
    pfcs = getPFCS();

    pfcs.Reciprocal();

    std::complex<double>* ptr = pfcs.GetDataPtr();
    EXPECT_NEAR(std::real(ptr[0]), 1.6, 0.01);
    EXPECT_NEAR(std::imag(ptr[0]), -0.8, 0.01);
    EXPECT_NEAR(std::real(ptr[7]), 0.282, 0.01);
    EXPECT_NEAR(std::imag(ptr[7]), -0.329, 0.01);
}

TEST(PressureFieldComplexSerialTest, conjugateTest)
{
    PressureFieldComplexSerial pfcs(getGrid());
    pfcs = getPFCS();
    pfcs.Conjugate();

    std::complex<double>* ptr = pfcs.GetDataPtr();
    EXPECT_EQ(std::imag(ptr[0]), -0.25);
    EXPECT_EQ(std::imag(ptr[7]), -1.75);
}

TEST(PressureFieldComplexSerialTest, normTest)
{
    PressureFieldComplexSerial pfcs(getGrid());
    pfcs = getPFCS();

    EXPECT_NEAR(pfcs.Norm(), 4.527, 0.1);
}

TEST(PressureFieldComplexSerialTest, relNormTest)
{
    PressureFieldComplexSerial pfcs(getGrid());
    pfcs = getPFCS();

    EXPECT_NEAR(pfcs.RelNorm(), 0.566, 0.1);
}

TEST(PressureFieldComplexSerial, innerProductTest)
{
    PressureFieldComplexSerial pfcs(getGrid());
    pfcs = getPFCS();

    std::complex<double> result = pfcs.InnerProduct(pfcs);
    
    EXPECT_NEAR(std::real(result), 20.5, 0.01);
    EXPECT_NEAR(std::imag(result), 0, 0.01);
}

TEST(PressureFieldComplexSerial, summationPFCSTest)
{
    PressureFieldComplexSerial pfcs(getGrid());
    pfcs = getPFCS();

    std::complex<double> result = pfcs.Summation(pfcs);
    
    EXPECT_NEAR(std::real(result), -0.5, 0.01);
    EXPECT_NEAR(std::imag(result), 16, 0.01);
}

TEST(PressureFieldComplexSerial, summationPFSTest)
{
    PressureFieldComplexSerial pfcs(getGrid());
    pfcs = getPFCS();

    PressureFieldSerial pfs(getGrid());
    std::function<double(double, double)> func = [](double x, double z){return x+z;}; // Linear tilted plane, x & z are centroids of grid cell.
    pfs.SetField(func);

    std::complex<double> result = pfcs.Summation(pfs);
    
    EXPECT_NEAR(std::real(result), 18, 0.01);
    EXPECT_NEAR(std::imag(result), 18.5, 0.01);
}

TEST(PressureFieldComplexSerial, getRealPartTest)
{
    PressureFieldComplexSerial pfcs(getGrid());
    pfcs = getPFCS();

    PressureFieldSerial pfs(getGrid());
    pfs = pfcs.GetRealPart();
    double* ptr = pfs.GetDataPtr();

    EXPECT_EQ(ptr[0], 0.5);
    EXPECT_EQ(ptr[1], 1.5);
}