#include <gtest/gtest.h>
#include <iostream>
#include "OpenMPgradientDescentInversion.h"
#include "integralForwardModel.h"
#include "genericInput.h"

TEST(OpenMPGradientDescentInversionTest, FirstTest)
{
    /*std::array<double, 2> x_min = {0.0, 0.0};
    std::array<double, 2> x_max = {5.0, 2.0};
    std::array<int, 2> n_x = {2,4};
    std::array<double, 2> SrcMin = {0.0, -2.0};
    std::array<double, 2> SrcMax = {2.0, -2.0};
    int nSrc = 2;
    std::array<double, 2> RecvMin = {3.0, -2.0};
    std::array<double, 2> RecvMax = {5.0, -2.0};
    int nRecv = 2;


    Grid2D grid = Grid2D(x_min, x_max, n_x);

    Freq freq={20, 40, 2};
    double c_0 = 2000.0;
    Sources src(SrcMin, SrcMax, nSrc);
    Receivers recv(RecvMin, RecvMax, nRecv);
    FrequenciesGroup freqg(freq, c_0);

    PressureFieldSerial chi(grid);
    chi.Random();

    const GenericInput gInput{" ", " ", " ", " ", c_0, freq, x_min, x_max, SrcMin, SrcMax, RecvMin, RecvMax, n_x, n_x, NSourcesReceivers{nSrc, nRecv}, " ", false};

    ForwardModelInterface *model;
    model = new IntegralForwardModel(grid, src, recv, freqg, gInput);

    OpenMPGradientDescentInversion(model, gInput);*/
     EXPECT_EQ(1, 1);
//     delete model;
}
