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