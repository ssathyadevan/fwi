#include <gtest/gtest.h>
#include "helmholtz2D.h"

#include "pressureFieldComplexSerial.h"
#include "grid2D.h"
#include "sources.h"
#include "pmlWidthFactor.h"


//Generic Input Card test:
TEST(helmholtz2DTest, testClass)
{
    std::array<int, 2> nx = {64,32};
    std::array<double, 2> upperLeft = {-300.0, 0.0};
    std::array<double, 2> lowerRight = {300.0, 300.0};

    std::array<double, 2> source1 = {-480.0, -5.0};
    std::array<double, 2> source2 = {480.0, -5.0};

    grid2D testGrid(upperLeft, lowerRight, nx);
    sources src( source1, source2, 2);

    pressureFieldSerial chi(testGrid);
    chi.Zero();

    PMLWidthFactor pmlWidth;
    pmlWidth.x = 0.5;
    pmlWidth.z = 0.5;
    Helmholtz2D Helmholtz10Hz(testGrid, 10.0, src, 2000.0, chi, pmlWidth);
    //Helmholtz10Hz.BuildMatrix(chi);

    pressureFieldComplexSerial pTot(testGrid);
    pTot = Helmholtz10Hz.Solve(source1, pTot);

    pTot.toFile("pTotTest.csv");

    EXPECT_EQ(64, 64);
}
