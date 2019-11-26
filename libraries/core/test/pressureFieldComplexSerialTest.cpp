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

PressureFieldComplexSerial getPFCS( Grid2D&  g )
{
    //Grid2D grid = getGrid();
    PressureFieldComplexSerial pfcs( g );
    std::function<std::complex<double>(double, double)> func = [](double a, double b){return std::complex<double>(a, b);}; // y = a + bi
    pfcs.SetField(func);

    return pfcs;
}

// TEST(pressureFieldComplexSerialTest, squareTest)
// {
// 	Grid2D g = getGrid();
//     PressureFieldComplexSerial pfcs( g );
//     pfcs = getPFCS( g );
//     /* Reason for initiating this way:
//     *       When doing: PressureFieldComplexSerial pfcs = getPFCS();
//     *       The copy-constructor is called, which uses memcpy().
//     *       In order to do succeed, both sides of the '=' sign must have equal grids already.
//     *       This is guaranteed by initialising an empty PF of 'size' [grid].
//     */
    
//     pfcs.Square();

//     std::complex<double>* ptr = pfcs.GetDataPtr();
//     EXPECT_NEAR(std::real(ptr[0]), 0.1875, 0.001);
//     EXPECT_NEAR(std::imag(ptr[0]), 0.25, 0.001);
//     EXPECT_NEAR(std::real(ptr[7]), -0.8125, 0.001);
//     EXPECT_NEAR(std::imag(ptr[7]), 5.25, 0.001);
// }

// TEST(pressureFieldComplexSerialTest, setFieldTest)
// {
//     PressureFieldComplexSerial pfcs(getGrid()); // Empty PFCS of size grid
//     std::function<std::complex<double>(double, double)> func = [](double a, double b){return std::complex<double>(a, b);};
//     pfcs.SetField(func);
    
//     std::complex<double>* ptr = pfcs.GetDataPtr();
//     EXPECT_EQ(sizeof(*ptr), 8*2);
//     EXPECT_EQ(std::real(ptr[0]), 0.5);
//     EXPECT_EQ(std::imag(ptr[0]), 0.25);
//     EXPECT_EQ(std::real(ptr[7]), 1.5);
//     EXPECT_EQ(std::imag(ptr[7]), 1.75);
// }

// TEST(pressureFieldComplexSerialTest, reciprocalTest)
// {
// 	Grid2D g = getGrid();
// 	PressureFieldComplexSerial pfcs(g);
// 	pfcs = getPFCS(g);

//     pfcs.Reciprocal();

//     std::complex<double>* ptr = pfcs.GetDataPtr();
//     EXPECT_NEAR(std::real(ptr[0]), 1.6, 0.01);
//     EXPECT_NEAR(std::imag(ptr[0]), -0.8, 0.01);
//     EXPECT_NEAR(std::real(ptr[7]), 0.282, 0.01);
//     EXPECT_NEAR(std::imag(ptr[7]), -0.329, 0.01);
// }

// TEST(PressureFieldComplexSerialTest, conjugateTest)
// {
// 	Grid2D g = getGrid();
// 	PressureFieldComplexSerial pfcs(g);
// 	pfcs = getPFCS(g);
//     pfcs.Conjugate();

//     std::complex<double>* ptr = pfcs.GetDataPtr();
//     EXPECT_EQ(std::imag(ptr[0]), -0.25);
//     EXPECT_EQ(std::imag(ptr[7]), -1.75);
// }

// TEST(PressureFieldComplexSerialTest, normTest)
// {
// 	Grid2D g = getGrid();
// 	PressureFieldComplexSerial pfcs(g);
// 	pfcs = getPFCS(g);

//     EXPECT_NEAR(pfcs.Norm(), 4.527, 0.1);
// }

// TEST(PressureFieldComplexSerialTest, relNormTest)
// {
// 	Grid2D g = getGrid();
// 	PressureFieldComplexSerial pfcs(g);
// 	pfcs = getPFCS(g);

//     EXPECT_NEAR(pfcs.RelNorm(), 0.566, 0.1);
// }

// TEST(PressureFieldComplexSerial, innerProductTest)
// {
// 	Grid2D g = getGrid();
// 	PressureFieldComplexSerial pfcs(g);
// 	pfcs = getPFCS(g);
//     std::complex<double> result = pfcs.InnerProduct(pfcs);
    
//     EXPECT_NEAR(std::real(result), 20.5, 0.01);
//     EXPECT_NEAR(std::imag(result), 0, 0.01);
// }

// TEST(PressureFieldComplexSerial, summationPFCSTest)
// {
// 	Grid2D g = getGrid();
// 	PressureFieldComplexSerial pfcs(g);
// 	pfcs = getPFCS(g);

//     std::complex<double> result = pfcs.Summation(pfcs);
    
//     EXPECT_NEAR(std::real(result), -0.5, 0.01);
//     EXPECT_NEAR(std::imag(result), 16, 0.01);
// }

// TEST(PressureFieldComplexSerial, summationPFSTest)
// {
// 	Grid2D g = getGrid();
// 	PressureFieldComplexSerial pfcs(g);
// 	pfcs = getPFCS(g);

//     PressureFieldSerial pfs(g);
//     std::function<double(double, double)> func = [](double x, double z){return x+z;}; // Linear tilted plane, x & z are centroids of grid cell.
//     pfs.SetField(func);

//     std::complex<double> result = pfcs.Summation(pfs);
    
//     EXPECT_NEAR(std::real(result), 18, 0.01);
//     EXPECT_NEAR(std::imag(result), 18.5, 0.01);
// }

// TEST(PressureFieldComplexSerial, getRealPartTest)
// {
// 	Grid2D g = getGrid();
// 	PressureFieldComplexSerial pfcs(g);
// 	pfcs = getPFCS(g);
//     PressureFieldSerial pfs(g);
//     pfs = pfcs.GetRealPart();
//     double* ptr = pfs.GetDataPtr();

//     EXPECT_EQ(ptr[0], 0.5);
//     EXPECT_EQ(ptr[1], 1.5);
// }

// TEST(PressureFieldComplexSerialTest, ZeroTest)
// {
//     using namespace std::complex_literals;

// 	Grid2D g = getGrid();
//     PressureFieldComplexSerial pfcs = getPFCS(g);
//     pfcs.Zero();
//     std::complex<double> *data_pfcs = pfcs.GetDataPtr();

//     std::complex<double> correctResult = 0i;
//     EXPECT_EQ(data_pfcs[1], correctResult);
// }

// TEST(PressureFieldComplexSerialTest, OperatorAssignPressureFieldComplexSerialTest)
// {
// 	Grid2D g = getGrid();
//     PressureFieldComplexSerial pfcs_1 = getPFCS(g);
//     PressureFieldComplexSerial pfcs_2 = pfcs_1;

//     std::complex<double> *data_pfcs_1 = pfcs_1.GetDataPtr();
//     std::complex<double> *data_pfcs_2 = pfcs_2.GetDataPtr();

//     for (int i = 0; i != pfcs_1.GetNumberOfGridPoints(); ++i)
//     {
//         EXPECT_EQ(data_pfcs_1[i], data_pfcs_2[i]);
//     }
// }

// TEST(PressureFieldComplexSerialTest, OperatorAssignDoubleTest)
// {
// 	Grid2D g = getGrid();
//     PressureFieldComplexSerial pfcs_1 = getPFCS(g);
//     pfcs_1 = 0.1;

//     std::complex<double> *data_pfcs_1 = pfcs_1.GetDataPtr();

//     for (int i = 0; i != pfcs_1.GetNumberOfGridPoints(); ++i)
//     {
//         EXPECT_EQ(data_pfcs_1[i], 0.1);
//     }
// }

// TEST(PressureFieldComplexSerialTest, OperatorSubtractPressureFieldComplexSerialTest)
// {
// 	Grid2D g1 = getGrid();
//     PressureFieldComplexSerial pfcs_1 = getPFCS(g1);
// 	// Grid2D g2 = getGrid();
//     PressureFieldComplexSerial pfcs_2 = getPFCS(g1);
//     pfcs_1 = 0.2;
//     pfcs_2 = 0.1;

//     pfcs_1 -= pfcs_2;

//     std::complex<double> *data_pfcs_1 = pfcs_1.GetDataPtr();

//     for (int i = 0; i != pfcs_1.GetNumberOfGridPoints(); ++i)
//     {
//         EXPECT_EQ(data_pfcs_1[i], 0.1);
//     }
// }

// TEST(PressureFieldComplexSerialTest, OperatorSubtractDoubleTest)
// {
// 	Grid2D g = getGrid();
//     PressureFieldComplexSerial pfcs_1 = getPFCS(g);
//     pfcs_1.Zero();
//     pfcs_1 -= 0.1;

//     std::complex<double> *data_pfcs_1 = pfcs_1.GetDataPtr();

//     for (int i = 0; i != pfcs_1.GetNumberOfGridPoints(); ++i)
//     {
//         EXPECT_EQ(data_pfcs_1[i], -0.1);
//     }
// }

// TEST(PressureFieldComplexSerialTest, OperatorDivideByDoubleTest)
// {
// 	Grid2D g = getGrid();
//     PressureFieldComplexSerial pfcs_1 = getPFCS(g);
//     pfcs_1 = 0.2;
//     pfcs_1 /= 2.0;

//     std::complex<double> *data_pfcs_1 = pfcs_1.GetDataPtr();

//     for (int i = 0; i != pfcs_1.GetNumberOfGridPoints(); ++i)
//     {
//         EXPECT_EQ(data_pfcs_1[i], 0.1);
//     }
// }

// TEST(PressureFieldComplexSerialTest, OperatorMultiplyByPressureComplexFieldSerialTest)
// {
// 	Grid2D g1 = getGrid();
//     PressureFieldComplexSerial pfcs_1 = getPFCS(g1);
// 	// Grid2D g2 = getGrid();
//     PressureFieldComplexSerial pfcs_2 = getPFCS(g1);
//     pfcs_1 = 0.2;
//     pfcs_2 = 2.0;
//     pfcs_1 *= pfcs_2;

//     std::complex<double> *data_pfcs_1 = pfcs_1.GetDataPtr();

//     for (int i = 0; i != pfcs_1.GetNumberOfGridPoints(); ++i)
//     {
//         EXPECT_EQ(data_pfcs_1[i], 0.4);
//     }
// }

// TEST(PressureFieldComplexSerialTest, OperatorMultiplyByDoubleTest)
// {
// 	Grid2D g = getGrid();
//     PressureFieldComplexSerial pfcs_1 = getPFCS(g);
//     pfcs_1 = 0.2;
//     pfcs_1 *= 2.0;

//     std::complex<double> *data_pfcs_1 = pfcs_1.GetDataPtr();

//     for (int i = 0; i != pfcs_1.GetNumberOfGridPoints(); ++i)
//     {
//         EXPECT_EQ(data_pfcs_1[i], 0.4);
//     }
// }