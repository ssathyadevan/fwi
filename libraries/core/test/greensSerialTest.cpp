#include "greensSerial.h"
#include <gtest/gtest.h>
#include <iostream>

namespace core
{
    greensRect2DCpu *getGreens()
    {
        std::array<double, 2> x_min = {0.0, 0.0};
        std::array<double, 2> x_max = {2.0, 2.0};
        std::array<int, 2> n_x = {2, 4};
        grid2D grid(x_min, x_max, n_x);

        std::function<std::complex<double>(double, double)> func = [](double a, double b) { return std::complex<double>(a, b); };   // y = a + bi

        x_max = {2.0, 0.0};
        sources sources(x_min, x_max, 3);

        x_max = {2.0, 0.0};
        receivers receivers(x_min, x_max, 3);

        double k = 1;

        greensRect2DCpu *greens = new greensRect2DCpu(grid, func, sources, receivers, k);

        return greens;
    }

    TEST(greensSerialTest, getGridTest)
    {
        greensRect2DCpu *greens = getGreens();
        EXPECT_EQ(greens->getGrid().getNumberOfGridPoints(), 8);
        delete greens;
    }

    TEST(greensSerialTest, getGreensVolumeTest)
    {
        greensRect2DCpu *greens = getGreens();

        const std::complex<double> *greensVol = greens->getGreensVolume();

        EXPECT_NEAR(std::real(*greensVol), 0.5, 0.01);
        EXPECT_NEAR(std::imag(*greensVol), 0.9, 0.01);

        delete greens;
    }

    TEST(greensSerialTest, getReceiverContTest)
    {
        greensRect2DCpu *greens = getGreens();

        const complexDataGrid2D *pfcs = greens->getReceiverCont(0);

        // Assert some property of pfcs
        double ip = pfcs->innerProduct(*pfcs);

        EXPECT_NEAR(ip, 7.125, 0.01);
    }
}   // namespace core
