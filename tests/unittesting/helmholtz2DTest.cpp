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

    //std::string path = "../../../tests/testCase/";
    //std::string path = "/home/leonard/parallelized-fwi/tests/testCase/";
    pressureFieldComplexSerial pythonBenchpTot(testGrid);

    pythonBenchpTot.fromFile("PythonBenchpTot.csv");

    //const std::complex<double>* lhs = pTot.GetDataPtr();
    //const std::complex<double>* rhs = pythonBenchpTot.GetDataPtr();

    std::cout << pTot.Norm() << std::endl;
    std::cout << pythonBenchpTot.Norm() << std::endl;

    pythonBenchpTot -= pTot;
    double res = pythonBenchpTot.RelNorm();

//    double res = 0.;
//    for (int i = 0; i < pTot.GetNumberOfGridPoints(); ++i) {
//        res += (lhs[i].real() - rhs[i].real())*(lhs[i].real() - rhs[i].real());
//    }
//    res = sqrt(res);

//    std::cout << res << std::endl;

    double tol = 1e-10;

    EXPECT_LT(res, tol);
}
