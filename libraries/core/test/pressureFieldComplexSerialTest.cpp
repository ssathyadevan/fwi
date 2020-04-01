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
TEST(PressureFieldComplexSerialTest, operatorAssignPressureFieldComplexSerialExceptionTest)
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

TEST(PressureFieldComplexSerialTest, operatorAssignPressureFieldComplexSerialTest)
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

TEST(PressureFieldComplexSerialTest, operatorAssignPressureFieldSerialTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    std::complex<double> testValue(0.0, 0.0);
    PressureFieldComplexSerial pfcs(grid);
    pfcs = testValue;

    PressureFieldSerial pfs(grid);
    // Note: Initialized with 0.0

    // When
    pfcs = pfs;

    // Then
    const std::vector<std::complex<double>> &pfcsData = pfcs.getData();
    const std::vector<double> &pfsData = pfs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(pfcsData[i].real(), pfsData[i]);
    }
}

TEST(PressureFieldComplexSerialTest, operatorAssignComplexDoubleVectorTest)
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

TEST(PressureFieldComplexSerialTest, operatorAssignComplexDoubleVectorTest2)
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
        count += increment;
    }

    // When
    pfcs = dataIncreasing;

    // Then
    count.real(0.0);
    count.imag(0.0);
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(count));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(count));
        count += increment;
    }
}

TEST(PressureFieldComplexSerialTest, operatorAssignDoubleVectorTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();
    PressureFieldComplexSerial pfcs(grid);

    double testValue = 1.0;
    std::vector<double> dataVector = std::vector<double>(nrOfGridPoints, testValue);
    // When
    pfcs = dataVector;

    // Then
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), testValue);
    }
}

TEST(PressureFieldComplexSerialTest, operatorAssignDoubleVectorTest2)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    double testValue = 1.0;
    PressureFieldComplexSerial pfcs(grid);
    pfcs = testValue;

    int count = 0;
    std::vector<double> dataIncreasing = std::vector<double>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count++;
    }

    // When
    pfcs = dataIncreasing;

    // Then
    count = 0;
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), count);
        count++;
    }
}

TEST(PressureFieldComplexSerialTest, operatorAssignComplexDoubleTest)
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

TEST(PressureFieldComplexSerialTest, operatorAssignDoubleTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();
    PressureFieldComplexSerial pfcs(grid);

    double testValue = 1.0;

    // When
    pfcs = testValue;

    // Then
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), testValue);
    }
}

TEST(PressureFieldComplexSerialTest, operatorAddPressureFieldComplexSerialTest)
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

TEST(PressureFieldComplexSerialTest, operatorAddPressureFieldComplexSerialTest2)
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
        count += increment;
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
        count += increment;
    }
}

TEST(PressureFieldComplexSerialTest, operatorAddPressureFieldSerialTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    std::complex<double> testValue1(1.0, 2.0);
    PressureFieldComplexSerial pfcs(grid);
    pfcs = testValue1;

    double testValue2 = 2.0;
    PressureFieldSerial pfs(grid);
    pfs = testValue2;

    // When
    pfcs += pfs;

    // Then
    std::complex<double> testSolution = testValue2 + testValue1;
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testSolution));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testSolution));
    }
}

TEST(PressureFieldComplexSerialTest, operatorAddPressureFieldSerialTest2)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    std::complex<double> testValue(1.0, 2.0);
    PressureFieldComplexSerial pfcs(grid);
    pfcs = testValue;

    double count = 0;
    std::vector<double> dataIncreasing = std::vector<double>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count++;
    }

    PressureFieldSerial pfs(grid);
    pfs = dataIncreasing;

    // When
    pfcs += pfs;

    // Then
    count = 0;
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(count + testValue));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(count + testValue));
        count++;
    }
}

TEST(PressureFieldComplexSerialTest, operatorAddComplexDoubleVectorTest)
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

TEST(PressureFieldComplexSerialTest, operatorAddComplexDoubleVectorTest2)
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
        count += increment;
    }

    // When
    pfcs += dataIncreasing;

    // Then
    count.real(0.0);
    count.imag(0.0);
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(count + testValue));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(count + testValue));
        count += increment;
    }
}

TEST(PressureFieldComplexSerialTest, operatorAddDoubleVectorTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    double testValue = 1.0;
    PressureFieldComplexSerial pfcs(grid);

    std::vector<double> dataVector = std::vector<double>(nrOfGridPoints, testValue);
    // When
    pfcs += dataVector;

    // Then
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), testValue);
    }
}

TEST(PressureFieldComplexSerialTest, operatorAddDoubleVectorTest2)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    std::complex<double> testValue(2.1, 3.1);
    PressureFieldComplexSerial pfcs(grid);
    pfcs = testValue;

    int count = 0;
    std::vector<double> dataIncreasing = std::vector<double>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count++;
    }

    // When
    pfcs += dataIncreasing;

    // Then
    count = 0;
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), (real(testValue) + count));
        count++;
    }
}

TEST(PressureFieldComplexSerialTest, operatorAddComplexDoubleTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    std::complex<double> testValue1(1.0, 2.0);
    PressureFieldComplexSerial pfcs(grid);
    pfcs = testValue1;

    // When
    std::complex<double> testValue2(2.0, 3.0);
    pfcs += testValue2;

    // Then
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testValue1 + testValue2));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testValue1 + testValue2));
    }
}

TEST(PressureFieldComplexSerialTest, operatorAddDoubleTest)
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

TEST(PressureFieldComplexSerialTest, operatorSubtractPressureFieldComplexSerialTest)
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

TEST(PressureFieldComplexSerialTest, operatorSubtractPressureFieldComplexSerialTest2)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    std::complex<double> testValue(21.0, 22.0);
    PressureFieldComplexSerial pfcs1(grid);
    pfcs1 = testValue;

    std::complex<double> count(0.0, 0.0);
    std::complex<double> increment(1.0, 1.0);
    std::vector<std::complex<double>> dataIncreasing = std::vector<std::complex<double>>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count += increment;
    }

    PressureFieldComplexSerial pfcs2(grid);
    pfcs2 = dataIncreasing;

    // When
    pfcs1 -= pfcs2;

    // Then
    count.real(0.0);
    count.imag(0.0);
    const std::vector<std::complex<double>> &data = pfcs1.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testValue - count));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testValue - count));
        count += increment;
    }
}

TEST(PressureFieldComplexSerialTest, operatorSubtractPressureFieldSerialTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    std::complex<double> testValue1(1.0, 2.0);
    PressureFieldComplexSerial pfcs(grid);
    pfcs = testValue1;

    double testValue2 = 2.0;
    PressureFieldSerial pfs(grid);
    pfs = testValue2;

    // When
    pfcs -= pfs;

    // Then
    std::complex<double> testSolution = testValue1 - testValue2;
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testSolution));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testSolution));
    }
}

TEST(PressureFieldComplexSerialTest, operatorSubtractPressureFieldSerialTest2)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    std::complex<double> testValue(1.0, 2.0);
    PressureFieldComplexSerial pfcs(grid);
    pfcs = testValue;

    double count = 0;
    std::vector<double> dataIncreasing = std::vector<double>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count++;
    }

    PressureFieldSerial pfs(grid);
    pfs = dataIncreasing;

    // When
    pfcs -= pfs;

    // Then
    count = 0.0;
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testValue - count));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testValue - count));
        count++;
    }
}

TEST(PressureFieldComplexSerialTest, operatorSubtractComplexDoubleVectorTest)
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

TEST(PressureFieldComplexSerialTest, operatorSubtractComplexDoubleVectorTest2)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    std::complex<double> testValue(21.0, 22.0);
    PressureFieldComplexSerial pfcs(grid);
    pfcs = testValue;

    std::complex<double> count(0.0, 0.0);
    std::complex<double> increment(1.0, 1.0);
    std::vector<std::complex<double>> dataIncreasing = std::vector<std::complex<double>>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count += increment;
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
        count += increment;
    }
}

TEST(PressureFieldComplexSerialTest, operatorSubtractDoubleVectorTest)
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

TEST(PressureFieldComplexSerialTest, operatorSubtractDoubleVectorTest2)
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
        count += increment;
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
        count += increment;
    }
}

TEST(PressureFieldComplexSerialTest, operatorSubtractComplexDoubleTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    std::complex<double> testValue1(31.0, 32.0);
    PressureFieldComplexSerial pfcs(grid);
    pfcs = testValue1;

    // When
    std::complex<double> testValue2(2.0, 3.0);
    pfcs -= testValue2;

    // Then
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testValue1 - testValue2));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testValue1 - testValue2));
    }
}

TEST(PressureFieldComplexSerialTest, operatorSubtractDoubleTest)
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

TEST(PressureFieldComplexSerialTest, operatorMultiplyByPressureFieldComplexSerialTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    std::complex<double> testValue1(2.1, 3.1);
    PressureFieldComplexSerial pfcs1(grid);
    pfcs1 = testValue1;

    std::complex<double> testValue2(3.1, 4.1);
    PressureFieldComplexSerial pfcs2(grid);
    pfcs2 = testValue2;

    // When
    pfcs2 *= pfcs1;

    // Then
    std::complex<double> testSolution = testValue2 * testValue1;
    const std::vector<std::complex<double>> &data = pfcs2.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testSolution));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testSolution));
    }
}

TEST(PressureFieldComplexSerialTest, operatorMultiplyByPressureFieldComplexSerialTest2)
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
        count += increment;
    }

    PressureFieldComplexSerial pfcs2(grid);
    pfcs2 = dataIncreasing;

    // When
    pfcs1 *= pfcs2;

    // Then
    count.real(0.0);
    count.imag(0.0);
    const std::vector<std::complex<double>> &data = pfcs1.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(count * testValue));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(count * testValue));
        count += increment;
    }
}

TEST(PressureFieldComplexSerialTest, operatorMultiplyByPressureFieldSerialTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    std::complex<double> testValue1(1.0, 2.0);
    PressureFieldComplexSerial pfcs(grid);
    pfcs = testValue1;

    double testValue2 = 2.0;
    PressureFieldSerial pfs(grid);
    pfs = testValue2;

    // When
    pfcs *= pfs;

    // Then
    std::complex<double> testSolution = testValue2 * testValue1;
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testSolution));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testSolution));
    }
}

TEST(PressureFieldComplexSerialTest, operatorMultiplyByPressureFieldSerialTest2)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    std::complex<double> testValue(1.0, 2.0);
    PressureFieldComplexSerial pfcs(grid);
    pfcs = testValue;

    double count = 0;
    std::vector<double> dataIncreasing = std::vector<double>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count++;
    }

    PressureFieldSerial pfs(grid);
    pfs = dataIncreasing;

    // When
    pfcs *= pfs;

    // Then
    count = 0;
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(count * testValue));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(count * testValue));
        count++;
    }
}

TEST(PressureFieldComplexSerialTest, operatorMultiplyByComplexDoubleVectorTest)
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
    pfcs *= dataVector;

    // Then
    std::complex<double> testSolution = testValue1 * testValue2;
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testSolution));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testSolution));
    }
}

TEST(PressureFieldComplexSerialTest, operatorMultiplyByComplexDoubleVectorTest2)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    std::complex<double> testValue(21.0, 22.0);
    PressureFieldComplexSerial pfcs(grid);
    pfcs = testValue;

    std::complex<double> count(0.0, 0.0);
    std::complex<double> increment(1.0, 1.0);
    std::vector<std::complex<double>> dataIncreasing = std::vector<std::complex<double>>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count += increment;
    }

    // When
    pfcs *= dataIncreasing;

    // Then
    count.real(0.0);
    count.imag(0.0);
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testValue * count));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testValue * count));
        count += increment;
    }
}

TEST(PressureFieldComplexSerialTest, operatorMultiplyByDoubleVectorTest)
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
    pfcs *= dataVector;

    // Then
    std::complex<double> testSolution = testValue1 * testValue2;
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testSolution));
    }
}

TEST(PressureFieldComplexSerialTest, operatorMultiplyByDoubleVectorTest2)
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
        count += increment;
    }

    // When
    pfcs *= dataIncreasing;

    // Then
    count.real(0.0);
    count.imag(0.0);
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testValue * count));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testValue * count));
        count += increment;
    }
}

TEST(PressureFieldComplexSerialTest, operatorMultiplyByComplexDoubleTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    std::complex<double> testValue1(2.2, 3.3);
    PressureFieldComplexSerial pfcs(grid);
    pfcs = testValue1;

    // When
    std::complex<double> testValue2(2.0, 3.0);
    pfcs *= testValue2;

    // Then
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testValue1 * testValue2));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testValue1 * testValue2));
    }
}

TEST(PressureFieldComplexSerialTest, operatorMultiplyByDoubleTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    std::complex<double> testValue1(2.1, 3.1);
    PressureFieldComplexSerial pfcs(grid);
    pfcs = testValue1;

    // When
    std::complex<double> testValue2(3.1, 4.1);
    pfcs *= testValue2;

    // Then
    std::complex<double> testSolution = testValue1 * testValue2;
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testSolution));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testSolution));
    }
}
