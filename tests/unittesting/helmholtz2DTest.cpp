#include <gtest/gtest.h>
#include "helmholtz2D.h"

#include "pressureFieldComplexSerial.h"
#include "grid2D.h"
#include "sources.h"
#include "finiteDifferenceForwardModelInput.h"


/* Test Finite Difference implementation of calculating pTot by comparing it to
 * a reference Python run */

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
    SourceParameter srcPar;
    srcPar.r = 4;
    srcPar.beta = 6.31;
    finiteDifferenceForwardModelInput fmInput;
    fmInput.pmlWidthFactor = pmlWidth;
    fmInput.sourceParameter = srcPar;
    Helmholtz2D Helmholtz10Hz(testGrid, 10.0, src, 2000.0, chi, fmInput);

    pressureFieldComplexSerial pTot(testGrid);
    pTot = Helmholtz10Hz.solve(source1, pTot);
    //pTot.toFile("pTotTest.csv");

    pressureFieldComplexSerial pythonBenchpTot(testGrid);

    std::string path = "../../../tests/testCase/";
    pythonBenchpTot.fromFile(path+"PythonBenchpTotNewSource.csv");
    //pythonBenchpTot.fromFile("PythonBenchpTotNewSource.csv");

    pressureFieldComplexSerial diff(pythonBenchpTot);
    diff = diff - pTot;
    double res = diff.Norm();
    double tol = 1e-10;

    EXPECT_LT(res, tol);
}
