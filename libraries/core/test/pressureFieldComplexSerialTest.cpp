#include "pressureFieldComplexSerial.h"
#include <gtest/gtest.h>
#include <iostream>

Grid2D getGrid()
{
    std::array<double, 2> x_min = {0.0, 0.0};
    std::array<double, 2> x_max = {2.0, 2.0};
    std::array<int, 2> n_x = {2, 4};

    Grid2D grid(x_min, x_max, n_x);
    return grid;
}

PressureFieldComplexSerial getPFCS(Grid2D &g)
{
    // Grid2D grid = getGrid();
    PressureFieldComplexSerial pfcs(g);
    std::function<std::complex<double>(double, double)> func = [](double a, double b) { return std::complex<double>(a, b); };   // y = a + bi
    pfcs.SetField(func);

    return pfcs;
}
TEST(PressureFieldComplexSerialTest, zeroTest)
{
    // Given
    Grid2D grid = getGrid();
    PressureFieldComplexSerial pfcs(grid);
    pfcs = 1.0;

    // When
    pfcs.Zero();

    // Then
    const int nrOfGridPoints = pfcs.GetNumberOfGridPoints();
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), 0.0);
        ASSERT_DOUBLE_EQ(data[i].imag(), 0.0);
    }
}

TEST(PressureFieldComplexSerialTest, squareTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    std::complex<double> testValue(1.0, 2.0);

    PressureFieldComplexSerial pfcs(grid);
    pfcs = testValue;

    // when
    pfcs.Square();

    // Then
    // std::complex<double> testSolution = sqrt(testValue);
    std::complex<double> testSolution = testValue * testValue;
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testSolution));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testSolution));
    }
}

TEST(PressureFieldComplexSerialTest, sqrtTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    std::complex<double> testValue(4.0, 9.0);
    PressureFieldComplexSerial pfcs(grid);
    pfcs = testValue;

    // When
    pfcs.Sqrt();

    // Then
    std::complex<double> testSolution = std::sqrt(testValue);
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testSolution));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testSolution));
    }
}

TEST(PressureFieldComplexSerialTest, normTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    std::complex<double> testValue(1.0, 2.0);
    PressureFieldComplexSerial pfcs(grid);
    pfcs = testValue;

    // Alternative calculation method
    std::complex<double> innerProduct(0.0, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        innerProduct += (testValue * testValue);
    }
    std::complex<double> sqrtInnerProduct = std::sqrt(innerProduct);

    // When
    std::complex<double> norm = pfcs.Norm();

    // Then
    ASSERT_DOUBLE_EQ(norm.real(), real(sqrtInnerProduct));
    ASSERT_DOUBLE_EQ(norm.imag(), imag(sqrtInnerProduct));
}

TEST(PressureFieldComplexSerialTest, relNormTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    double testValue = 2.0;
    PressureFieldSerial pfcs(grid);
    pfcs = testValue;

    // Alternative calculation method
    std::complex<double> innerProduct(0.0, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        innerProduct += testValue * testValue;
    }
    std::complex<double> nrOfGridPointsComplex(nrOfGridPoints, nrOfGridPoints);
    std::complex<double> dividedInnerProduct = innerProduct / nrOfGridPointsComplex;
    std::complex<double> sqrtDividedInnerProduct = std::sqrt(dividedInnerProduct);

    // When
    std::complex<double> relNorm = pfcs.RelNorm();

    // Then
    ASSERT_DOUBLE_EQ(relNorm.real(), real(sqrtDividedInnerProduct));
    ASSERT_DOUBLE_EQ(relNorm.imag(), imag(sqrtDividedInnerProduct));
}

TEST(pressureFieldComplexSerialTest, reciprocalTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    std::complex<double> testValue(1.0, 2.0);
    PressureFieldComplexSerial pfcs(grid);
    pfcs = testValue;

    // When
    pfcs.Reciprocal();

    // Then
    std::complex<double> testSolution = 1.0 / testValue;
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testSolution));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testSolution));
    }
}

TEST(PressureFieldComplexSerialTest, reciprocalExceptionTest)
{
    // Given
    Grid2D grid = getGrid();
    PressureFieldComplexSerial pfcs(grid);
    // Note: Initialized with zero

    // When
    // Nothing here

    // Then
    EXPECT_THROW(pfcs.Reciprocal(), std::overflow_error);
    // Note: 1 / 0.0 must throw
}

TEST(PressureFieldComplexSerialTest, summationTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    std::complex<double> testValue = 2.0;
    PressureFieldComplexSerial pfcs(grid);
    pfcs = testValue;

    // Alternative calculation method
    std::complex<double> summedValue = 0.0;
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        summedValue += testValue;
    }

    // When
    std::complex<double> summationResult = pfcs.Summation();

    // Then
    ASSERT_DOUBLE_EQ(summationResult.real(), real(summedValue));
    ASSERT_DOUBLE_EQ(summationResult.imag(), imag(summedValue));
}

// Non virtual members
TEST(PressureFieldComplexSerialTest, setDataTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    PressureFieldComplexSerial pfcs(grid);

    // Create vextor with increasing values
    int count = 0;
    std::vector<std::complex<double>> dataIncreasing = std::vector<std::complex<double>>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count++;
    }

    // When
    pfcs.setData(dataIncreasing);

    // Then
    count = 0;
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), count);
        ASSERT_DOUBLE_EQ(data[i].imag(), count);
        count++;
    }
}

TEST(PressureFieldComplexSerialTest, addValueAtIndexTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    std::complex<double> testValue = 13;
    int index = 3;
    PressureFieldComplexSerial pfcs(grid);

    // When
    pfcs.addValueAtIndex(testValue, index);

    // Then
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        if(i != index)
        {
            ASSERT_DOUBLE_EQ(data[i].real(), 0);
            ASSERT_DOUBLE_EQ(data[i].imag(), 0);
        }
        else
        {
            ASSERT_DOUBLE_EQ(data[i].real(), real(testValue));
            ASSERT_DOUBLE_EQ(data[i].imag(), imag(testValue));
        }
    }
}

TEST(PressureFieldComplexSerialTest, innerProductTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    std::complex<double> testValue = 2.0;
    PressureFieldComplexSerial pfcs(grid);
    pfcs = testValue;

    // Alternative calculation method
    std::complex<double> alternativeInnerProduct = 0.0;
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        alternativeInnerProduct += testValue * testValue;
    }

    // When
    std::complex<double> calculatedInerProduct = pfcs.InnerProduct(pfcs);

    // Then
    ASSERT_DOUBLE_EQ(calculatedInerProduct.real(), real(alternativeInnerProduct));
    ASSERT_DOUBLE_EQ(calculatedInerProduct.imag(), imag(alternativeInnerProduct));
}

// Operators
TEST(PressureFieldSerialTest, operatorAssignPressureFieldSerialExceptionTest)
{
    // Given
    Grid2D grid = getGrid();
    PressureFieldComplexSerial pfcs(grid);

    // When
    // Nothing here, see below

    // Then
    EXPECT_THROW(pfcs = pfcs, std::logic_error);
    // Note: 1 / 0.0 must throw
}

TEST(PressureFieldComplexSerialTest, conjugateTest)
{
    Grid2D g = getGrid();
    PressureFieldComplexSerial pfcs(g);
    pfcs = getPFCS(g);
    pfcs.Conjugate();

    std::complex<double> *ptr = pfcs.GetDataPtr();
    EXPECT_EQ(std::imag(ptr[0]), -0.25);
    EXPECT_EQ(std::imag(ptr[7]), -1.75);
}

TEST(PressureFieldComplexSerial, getRealPartTest)
{
    Grid2D g = getGrid();
    PressureFieldComplexSerial pfcs(g);
    pfcs = getPFCS(g);
    PressureFieldSerial pfs(g);
    pfs = pfcs.GetRealPart();
    double *ptr = pfs.GetDataPtr();

    EXPECT_EQ(ptr[0], 0.5);
    EXPECT_EQ(ptr[1], 1.5);
}

TEST(PressureFieldComplexSerialTest, OperatorAssignPressureFieldComplexSerialTest)
{
    Grid2D g = getGrid();
    PressureFieldComplexSerial pfcs_1 = getPFCS(g);
    PressureFieldComplexSerial pfcs_2 = pfcs_1;

    std::complex<double> *data_pfcs_1 = pfcs_1.GetDataPtr();
    std::complex<double> *data_pfcs_2 = pfcs_2.GetDataPtr();

    for(int i = 0; i != pfcs_1.GetNumberOfGridPoints(); ++i)
    {
        EXPECT_EQ(data_pfcs_1[i], data_pfcs_2[i]);
    }
}

TEST(PressureFieldComplexSerialTest, OperatorAssignDoubleTest)
{
    Grid2D g = getGrid();
    PressureFieldComplexSerial pfcs_1 = getPFCS(g);
    pfcs_1 = 0.1;

    std::complex<double> *data_pfcs_1 = pfcs_1.GetDataPtr();

    for(int i = 0; i != pfcs_1.GetNumberOfGridPoints(); ++i)
    {
        EXPECT_EQ(data_pfcs_1[i], 0.1);
    }
}

TEST(PressureFieldComplexSerialTest, OperatorSubtractPressureFieldComplexSerialTest)
{
    Grid2D g1 = getGrid();
    PressureFieldComplexSerial pfcs_1 = getPFCS(g1);
    PressureFieldComplexSerial pfcs_2 = getPFCS(g1);
    pfcs_1 = 0.2;
    pfcs_2 = 0.1;

    pfcs_1 -= pfcs_2;

    std::complex<double> *data_pfcs_1 = pfcs_1.GetDataPtr();

    for(int i = 0; i != pfcs_1.GetNumberOfGridPoints(); ++i)
    {
        EXPECT_EQ(data_pfcs_1[i], 0.1);
    }
}

TEST(PressureFieldComplexSerialTest, OperatorSubtractDoubleTest)
{
    Grid2D g = getGrid();
    PressureFieldComplexSerial pfcs_1 = getPFCS(g);
    pfcs_1.Zero();
    pfcs_1 -= 0.1;

    std::complex<double> *data_pfcs_1 = pfcs_1.GetDataPtr();

    for(int i = 0; i != pfcs_1.GetNumberOfGridPoints(); ++i)
    {
        EXPECT_EQ(data_pfcs_1[i], -0.1);
    }
}

TEST(PressureFieldComplexSerialTest, OperatorDivideByDoubleTest)
{
    Grid2D g = getGrid();
    PressureFieldComplexSerial pfcs_1 = getPFCS(g);
    pfcs_1 = 0.2;
    pfcs_1 /= 2.0;

    std::complex<double> *data_pfcs_1 = pfcs_1.GetDataPtr();

    for(int i = 0; i != pfcs_1.GetNumberOfGridPoints(); ++i)
    {
        EXPECT_EQ(data_pfcs_1[i], 0.1);
    }
}

TEST(PressureFieldComplexSerialTest, OperatorMultiplyByPressureComplexFieldSerialTest)
{
    Grid2D g1 = getGrid();
    PressureFieldComplexSerial pfcs_1 = getPFCS(g1);
    // Grid2D g2 = getGrid();
    PressureFieldComplexSerial pfcs_2 = getPFCS(g1);
    pfcs_1 = 0.2;
    pfcs_2 = 2.0;
    pfcs_1 *= pfcs_2;

    std::complex<double> *data_pfcs_1 = pfcs_1.GetDataPtr();

    for(int i = 0; i != pfcs_1.GetNumberOfGridPoints(); ++i)
    {
        EXPECT_EQ(data_pfcs_1[i], 0.4);
    }
}

TEST(PressureFieldComplexSerialTest, OperatorMultiplyByDoubleTest)
{
    Grid2D g = getGrid();
    PressureFieldComplexSerial pfcs_1 = getPFCS(g);
    pfcs_1 = 0.2;
    pfcs_1 *= 2.0;

    std::complex<double> *data_pfcs_1 = pfcs_1.GetDataPtr();

    for(int i = 0; i != pfcs_1.GetNumberOfGridPoints(); ++i)
    {
        EXPECT_EQ(data_pfcs_1[i], 0.4);
    }
}
