#include "pressureFieldComplexSerial.h"
#include <gtest/gtest.h>
#include <iostream>

Grid2D getGrid()
{
    std::array<double, 2> _xMin = {0.0, 0.0};
    std::array<double, 2> _xMax = {2.0, 2.0};
    std::array<int, 2> _nX = {2, 4};

    Grid2D grid(_xMin, _xMax, _nX);
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

/*
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
        innerProduct += pow(testValue, 2);
    }

    std::complex<double> realInnerProduct = std::real(innerProduct);
    std::complex<double> sqrtRealInnerProduct2 = sqrt(realInnerProduct);

    // When
    std::complex<double> norm = pfcs.Norm();

    // Then
    ASSERT_DOUBLE_EQ(norm.real(), real(sqrtRealInnerProduct));
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
        innerProduct += pow(testValue, 2);
    }
    std::complex<double> nrOfGridPointsComplex(nrOfGridPoints, nrOfGridPoints);
    std::complex<double> dividedRealInnerProduct = std::real(innerProduct / nrOfGridPointsComplex);
    std::complex<double> sqrtDividedRealInnerProduct = std::sqrt(dividedRealInnerProduct);

    // When
    std::complex<double> relNorm = pfcs.RelNorm();

    // Then
    ASSERT_DOUBLE_EQ(relNorm.real(), real(sqrtDividedRealInnerProduct));
}
*/

TEST(PressureFieldComplexSerialTest, reciprocalTest)
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
TEST(PressureFieldComplexSerialTest, operatorAssignPressureFieldSerialExceptionTest)
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

TEST(PressureFieldComplexSerialTest, operatorAssignPressureFieldSerialTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    std::complex<double> testValue(0.0, 0.0);
    PressureFieldComplexSerial pfcs1(grid);
    pfcs1 = testValue;

    PressureFieldComplexSerial pfcs2(grid);
    // Note: Initialized with 0.0

    // When
    pfcs2 = pfcs1;

    // Then
    const std::vector<std::complex<double>> &pfcs1Data = pfcs1.getData();
    const std::vector<std::complex<double>> &pfcs2Data = pfcs2.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(pfcs1Data[i].real(), pfcs2Data[i].real());
        ASSERT_DOUBLE_EQ(pfcs2Data[i].real(), real(testValue));
        ASSERT_DOUBLE_EQ(pfcs1Data[i].imag(), pfcs2Data[i].imag());
        ASSERT_DOUBLE_EQ(pfcs2Data[i].imag(), imag(testValue));
    }
}

TEST(PressureFieldComplexSerialTest, operatorAssignDoubleTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();
    PressureFieldComplexSerial pfcs(grid);

    std::complex<double> testValue(1.0, 2.0);

    // When
    pfcs = testValue;

    // Then
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testValue));
    }
}

TEST(PressureFieldComplexSerialTest, operatorAssignDoubleVectorTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();
    PressureFieldComplexSerial pfcs(grid);

    std::complex<double> testValue(1.0, 2.0);
    std::vector<std::complex<double>> dataVector = std::vector<std::complex<double>>(nrOfGridPoints, testValue);

    // When
    pfcs = dataVector;

    // Then
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testValue));
    }
}

TEST(PressureFieldComplexSerialTest, OperatorAssignDoubleVectorTest2)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    std::complex<double> testValue(1.0, 2.0);
    PressureFieldComplexSerial pfcs(grid);
    pfcs = testValue;

    std::complex<double> count(0.0, 0.0);
    std::complex<double> increment(1.0, 1.0);
    std::vector<std::complex<double>> dataIncreasing = std::vector<std::complex<double>>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count = count + increment;
    }

    // When
    pfcs = dataIncreasing;

    // Then
    count = 0;
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(count));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(count));
        count = count + increment;
    }
}

TEST(PressureFieldComplexSerialTest, OperatorAddPressureFieldSerialTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    std::complex<double> testValue1(1.0, 2.0);
    PressureFieldComplexSerial pfcs1(grid);
    pfcs1 = testValue1;

    std::complex<double> testValue2(2.0, 3.0);
    PressureFieldComplexSerial pfcs2(grid);
    pfcs2 = testValue2;

    // When
    pfcs2 += pfcs1;

    // Then
    std::complex<double> testSolution = testValue2 + testValue1;
    const std::vector<std::complex<double>> &data = pfcs2.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testSolution));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testSolution));
    }
}

TEST(PressureFieldComplexSerialTest, OperatorAddPressureFieldSerialTest2)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    std::complex<double> testValue(1.0, 2.0);
    PressureFieldComplexSerial pfcs1(grid);
    pfcs1 = testValue;

    std::complex<double> count(0.0, 0.0);
    std::complex<double> increment(1.0, 1.0);

    std::vector<std::complex<double>> dataIncreasing = std::vector<std::complex<double>>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count = count + increment;
    }

    PressureFieldComplexSerial pfcs2(grid);
    pfcs2 = dataIncreasing;

    // When
    pfcs2 += pfcs1;

    // Then
    count.real(0.0);
    count.imag(0.0);
    const std::vector<std::complex<double>> &data = pfcs2.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(count + testValue));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(count + testValue));
        count = count + increment;
    }
}

TEST(PressureFieldComplexSerialTest, OperatorAddDoubleVectorTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    std::complex<double> testValue1(2.0, 3.0);
    PressureFieldComplexSerial pfcs(grid);
    pfcs = testValue1;

    std::complex<double> testValue2(3.1, 4.2);
    std::vector<std::complex<double>> dataVector = std::vector<std::complex<double>>(nrOfGridPoints, testValue2);

    // When
    pfcs += dataVector;

    // Then
    std::complex<double> testSolution = testValue1 + testValue2;
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testSolution));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testSolution));
    }
}

TEST(PressureFieldComplexSerialTest, OperatorAddDoubleVectorTest2)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    std::complex<double> testValue(2.1, 3.1);
    PressureFieldComplexSerial pfcs(grid);
    pfcs = testValue;

    std::complex<double> count(0.0, 0.0);
    std::complex<double> increment(1.0, 1.0);

    std::vector<std::complex<double>> dataIncreasing = std::vector<std::complex<double>>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count = count + increment;
    }

    // When
    pfcs += dataIncreasing;

    // Then
    count.real(0.0);
    count.imag(0.0);
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testValue + count));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testValue + count));
        count = count + increment;
    }
}

TEST(PressureFieldComplexSerialTest, OperatorAddDoubleTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    std::complex<double> testValue1(2.1, 3.1);
    PressureFieldComplexSerial pfcs(grid);
    pfcs = testValue1;

    // When
    std::complex<double> testValue2(2.2, 3.2);
    pfcs += testValue2;

    // Then
    std::complex<double> testSolution = testValue1 + testValue2;
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testSolution));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testSolution));
    }
}

TEST(PressureFieldComplexSerialTest, OperatorSubtractPressureFieldSerialTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    std::complex<double> testValue1 = 9.2;
    PressureFieldComplexSerial pfcs1(grid);
    pfcs1 = testValue1;

    std::complex<double> testValue2 = 4.8;
    PressureFieldComplexSerial pfcs2(grid);
    pfcs2 = testValue2;

    // When
    pfcs2 -= pfcs1;

    // Then
    std::complex<double> testSolution = testValue2 - testValue1;
    const std::vector<std::complex<double>> &data = pfcs2.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testSolution));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testSolution));
    }
}

TEST(PressureFieldComplexSerialTest, OperatorSubtractPressureFieldSerialTest2)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    std::complex<double> testValue(2.1, 3.1);
    PressureFieldComplexSerial pfcs1(grid);
    pfcs1 = testValue;

    std::complex<double> count(0.0, 0.0);
    std::complex<double> increment(1.0, 1.0);

    std::vector<std::complex<double>> dataIncreasing = std::vector<std::complex<double>>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count = count + increment;
    }

    PressureFieldComplexSerial pfcs2(grid);
    pfcs2 = dataIncreasing;

    // When
    pfcs2 -= pfcs1;

    // Then
    count.real(0.0);
    count.imag(0.0);
    const std::vector<std::complex<double>> &data = pfcs2.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(count - testValue));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(count - testValue));
        count = count + increment;
    }
}

TEST(PressureFieldComplexSerialTest, OperatorSubtractDoubleVectorTest2)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    std::complex<double> testValue(2.1, 3.1);
    PressureFieldComplexSerial pfcs(grid);
    pfcs = testValue;

    std::complex<double> count(0.0, 0.0);
    std::complex<double> increment(1.0, 1.0);

    std::vector<std::complex<double>> dataIncreasing = std::vector<std::complex<double>>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count = count + increment;
    }

    // When
    pfcs -= dataIncreasing;

    // Then
    count.real(0.0);
    count.imag(0.0);
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testValue - count));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testValue - count));
        count = count + increment;
    }
}

TEST(PressureFieldComplexSerialTest, OperatorSubtractDoubleVectorTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    std::complex<double> testValue1(2.1, 3.1);
    PressureFieldComplexSerial pfcs(grid);
    pfcs = testValue1;

    std::complex<double> testValue2(3.1, 4.1);
    std::vector<std::complex<double>> dataVector = std::vector<std::complex<double>>(nrOfGridPoints, testValue2);

    // When
    pfcs -= dataVector;

    // Then
    std::complex<double> testSolution = testValue1 - testValue2;
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testSolution));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testSolution));
    }
}

TEST(PressureFieldComplexSerialTest, OperatorSubtractDoubleTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    std::complex<double> testValue1(2.1, 3.1);
    PressureFieldComplexSerial pfcs(grid);
    pfcs = testValue1;

    // When
    std::complex<double> testValue2(3.1, 4.1);
    pfcs -= testValue2;

    // Then
    std::complex<double> testSolution = testValue1 - testValue2;
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testSolution));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testSolution));
    }
}
