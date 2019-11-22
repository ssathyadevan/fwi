#include <gtest/gtest.h>
#include "greensSerial.h"
#include <iostream>

// Grid2D grid()
// {
    
// }

Greens_rect_2D_cpu* getGreens()
{
    std::array<double, 2> x_min = {0.0, 0.0};
    std::array<double, 2> x_max = {2.0, 2.0};
    std::array<int, 2> n_x = {2,4};
    Grid2D grid(x_min, x_max, n_x);

    std::function<std::complex<double>(double, double)> func = [](double a, double b){return std::complex<double>(a, b);}; // y = a + bi

    x_max = {2.0, 0.0};
    Sources sources(x_min, x_max, 3);

    x_max = {2.0, 0.0};
    Receivers receivers(x_min, x_max, 3);

    double k = 1;

    Greens_rect_2D_cpu* greens = new Greens_rect_2D_cpu(grid, func, sources, receivers, k);
    
    return greens;
}

TEST(greensSerialTest, getGridTest)
{
    Greens_rect_2D_cpu* greens = getGreens();
    EXPECT_EQ(greens->GetGrid().GetNumberOfGridPoints(), 8);
    delete greens;
}

TEST(greensSerialTest, getGreensVolumeTest)
{
    Greens_rect_2D_cpu* greens = getGreens();
    
    const std::complex<double>* greensVol = greens->GetGreensVolume();

    EXPECT_NEAR(std::real(*greensVol), 0.5, 0.01);
    EXPECT_NEAR(std::imag(*greensVol), 0.9, 0.01);

    delete greens;
}

TEST(greensSerialTest, getReceiverContTest)
{
    Greens_rect_2D_cpu* greens = getGreens();

    const PressureFieldComplexSerial* pfcs = greens->GetReceiverCont(0);

    // Assert some property of pfcs
    std::complex<double> ip = pfcs->InnerProduct(*pfcs);

    EXPECT_NEAR(std::real(ip), 7.125, 0.01);
    EXPECT_NEAR(std::imag(ip), 0, 0.01);
}

/* In Progress.
TEST(greensSerialTest, contractWithFieldTest)
{
    Greens_rect_2D_cpu* greens = getGreens();
    
    Grid2D grid = greens->GetGrid();

    PressureFieldComplexSerial pfcs_1(grid);
    std::function<std::complex<double>(double, double)> func = [](double a, double b){return std::complex<double>(a, b);}; // y = a + bi
    pfcs_1.SetField(func);
    
    PressureFieldComplexSerial pfcs_2(grid);
    pfcs_2 = greens->ContractWithField(pfcs_1);
    std::complex<double>* ptr = pfcs_2.GetDataPtr();

    for(int i=0; i<pfcs_2.GetNumberOfGridPoints(); i++)
    {
        std::cout << ptr[i] << std::endl;
    }

    std::complex<double> ip = pfcs_2.InnerProduct(pfcs_2);

    std::cout << ip << std::endl;
}*/