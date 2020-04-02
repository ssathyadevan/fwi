#include "pressureFieldComplexSerial.h"
#include <gtest/gtest.h>
#include <iostream>

grid2D getGrid()
{
    std::array<double, 2> xMin = {0.0, 0.0};
    std::array<double, 2> xMax = {2.0, 2.0};
    std::array<int, 2> nX = {2, 4};

    grid2D grid(xMin, xMax, nX);
    return grid;
}

TEST(pressureFieldComplexSerialTest, zeroTest)
{
    // Given
    grid2D grid = getGrid();

    std::complex<double> testValue(1.0, 1.0);
    pressureFieldComplexSerial pfcs(grid);
    pfcs = testValue;

    // When
    pfcs.zero();

    // Then
    const int nrOfGridPoints = pfcs.getNumberOfGridPoints();
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), 0.0);
        ASSERT_DOUBLE_EQ(data[i].imag(), 0.0);
    }
}

TEST(pressureFieldComplexSerialTest, squareTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue(1.0, 2.0);
    pressureFieldComplexSerial pfcs(grid);
    pfcs = testValue;

    // when
    pfcs.square();

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

TEST(pressureFieldComplexSerialTest, sqrtTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue(4.0, 9.0);
    pressureFieldComplexSerial pfcs(grid);
    pfcs = testValue;

    // When
    pfcs.sqrt();

    // Then
    std::complex<double> testSolution = std::sqrt(testValue);
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testSolution));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testSolution));
    }
}

TEST(pressureFieldComplexSerialTest, reciprocalTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue(1.0, 2.0);
    pressureFieldComplexSerial pfcs(grid);
    pfcs = testValue;

    // When
    pfcs.reciprocal();

    // Then
    std::complex<double> testSolution = 1.0 / testValue;
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testSolution));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testSolution));
    }
}

TEST(pressureFieldComplexSerialTest, reciprocalExceptionTest)
{
    // Given
    grid2D grid = getGrid();
    pressureFieldComplexSerial pfcs(grid);
    // Note: Initialized with zero

    // When
    // Nothing here

    // Then
    EXPECT_THROW(pfcs.reciprocal(), std::overflow_error);
    // Note: 1 / 0.0 must throw
}

TEST(pressureFieldComplexSerialTest, summationTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue = 2.0;
    pressureFieldComplexSerial pfcs(grid);
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
TEST(pressureFieldComplexSerialTest, addValueAtIndexTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue = 13;
    int index = 3;
    pressureFieldComplexSerial pfcs(grid);

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
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue = {2.0, 1.0};
    pressureFieldComplexSerial pfcs(grid);
    pfcs = testValue;

    // Alternative calculation method
    double alternativeInnerProduct = 0.0;
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        alternativeInnerProduct += real(testValue) * real(testValue) + imag(testValue) * imag(testValue);
    }

    // When
    double calculatedInerProduct = pfcs.innerProduct(pfcs);

    // Then
    ASSERT_DOUBLE_EQ(calculatedInerProduct, alternativeInnerProduct);
}

// Operators
TEST(pressureFieldComplexSerialTest, operatorAssignPressureFieldComplexSerialExceptionTest)
{
    // Given
    grid2D grid = getGrid();
    pressureFieldComplexSerial pfcs(grid);

    // When
    // Nothing here, see below

    // Then
    EXPECT_THROW(pfcs = pfcs, std::logic_error);
}

TEST(pressureFieldComplexSerialTest, operatorAssignPressureFieldComplexSerialTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue(0.0, 0.0);
    pressureFieldComplexSerial pfcs1(grid);
    pfcs1 = testValue;

    pressureFieldComplexSerial pfcs2(grid);
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

TEST(pressureFieldComplexSerialTest, operatorAssignPressureFieldSerialTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue(0.0, 0.0);
    pressureFieldComplexSerial pfcs(grid);
    pfcs = testValue;

    pressureFieldSerial pfs(grid);
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

TEST(pressureFieldComplexSerialTest, operatorAssignComplexDoubleVectorTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();
    pressureFieldComplexSerial pfcs(grid);

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

TEST(pressureFieldComplexSerialTest, operatorAssignComplexDoubleVectorTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue(1.0, 2.0);
    pressureFieldComplexSerial pfcs(grid);
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

TEST(pressureFieldComplexSerialTest, operatorAssignDoubleVectorTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();
    pressureFieldComplexSerial pfcs(grid);

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

TEST(pressureFieldComplexSerialTest, operatorAssignDoubleVectorTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double testValue = 1.0;
    pressureFieldComplexSerial pfcs(grid);
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

TEST(pressureFieldComplexSerialTest, operatorAssignComplexDoubleTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();
    pressureFieldComplexSerial pfcs(grid);

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

TEST(pressureFieldComplexSerialTest, operatorAssignDoubleTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();
    pressureFieldComplexSerial pfcs(grid);

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

TEST(pressureFieldComplexSerialTest, operatorAddPressureFieldComplexSerialTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(1.0, 2.0);
    pressureFieldComplexSerial pfcs1(grid);
    pfcs1 = testValue1;

    std::complex<double> testValue2(2.0, 3.0);
    pressureFieldComplexSerial pfcs2(grid);
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

TEST(pressureFieldComplexSerialTest, operatorAddPressureFieldComplexSerialTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue(1.0, 2.0);
    pressureFieldComplexSerial pfcs1(grid);
    pfcs1 = testValue;

    std::complex<double> count(0.0, 0.0);
    std::complex<double> increment(1.0, 1.0);
    std::vector<std::complex<double>> dataIncreasing = std::vector<std::complex<double>>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count += increment;
    }

    pressureFieldComplexSerial pfcs2(grid);
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

TEST(pressureFieldComplexSerialTest, operatorAddPressureFieldSerialTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(1.0, 2.0);
    pressureFieldComplexSerial pfcs(grid);
    pfcs = testValue1;

    double testValue2 = 2.0;
    pressureFieldSerial pfs(grid);
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

TEST(pressureFieldComplexSerialTest, operatorAddPressureFieldSerialTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue(1.0, 2.0);
    pressureFieldComplexSerial pfcs(grid);
    pfcs = testValue;

    double count = 0;
    std::vector<double> dataIncreasing = std::vector<double>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count++;
    }

    pressureFieldSerial pfs(grid);
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

TEST(pressureFieldComplexSerialTest, operatorAddComplexDoubleVectorTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(2.0, 3.0);
    pressureFieldComplexSerial pfcs(grid);
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

TEST(pressureFieldComplexSerialTest, operatorAddComplexDoubleVectorTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue(1.0, 2.0);
    pressureFieldComplexSerial pfcs(grid);
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

TEST(pressureFieldComplexSerialTest, operatorAddDoubleVectorTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double testValue = 1.0;
    pressureFieldComplexSerial pfcs(grid);

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

TEST(pressureFieldComplexSerialTest, operatorAddDoubleVectorTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue(2.1, 3.1);
    pressureFieldComplexSerial pfcs(grid);
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

TEST(pressureFieldComplexSerialTest, operatorAddComplexDoubleTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(1.0, 2.0);
    pressureFieldComplexSerial pfcs(grid);
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

TEST(pressureFieldComplexSerialTest, operatorAddDoubleTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(2.1, 3.1);
    pressureFieldComplexSerial pfcs(grid);
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

TEST(pressureFieldComplexSerialTest, operatorSubtractPressureFieldComplexSerialTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1 = 9.2;
    pressureFieldComplexSerial pfcs1(grid);
    pfcs1 = testValue1;

    std::complex<double> testValue2 = 4.8;
    pressureFieldComplexSerial pfcs2(grid);
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

TEST(pressureFieldComplexSerialTest, operatorSubtractPressureFieldComplexSerialTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue(21.0, 22.0);
    pressureFieldComplexSerial pfcs1(grid);
    pfcs1 = testValue;

    std::complex<double> count(0.0, 0.0);
    std::complex<double> increment(1.0, 1.0);
    std::vector<std::complex<double>> dataIncreasing = std::vector<std::complex<double>>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count += increment;
    }

    pressureFieldComplexSerial pfcs2(grid);
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

TEST(pressureFieldComplexSerialTest, operatorSubtractPressureFieldSerialTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(1.0, 2.0);
    pressureFieldComplexSerial pfcs(grid);
    pfcs = testValue1;

    double testValue2 = 2.0;
    pressureFieldSerial pfs(grid);
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

TEST(pressureFieldComplexSerialTest, operatorSubtractPressureFieldSerialTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue(1.0, 2.0);
    pressureFieldComplexSerial pfcs(grid);
    pfcs = testValue;

    double count = 0;
    std::vector<double> dataIncreasing = std::vector<double>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count++;
    }

    pressureFieldSerial pfs(grid);
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

TEST(pressureFieldComplexSerialTest, operatorSubtractComplexDoubleVectorTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(2.0, 3.0);
    pressureFieldComplexSerial pfcs(grid);
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

TEST(pressureFieldComplexSerialTest, operatorSubtractComplexDoubleVectorTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue(21.0, 22.0);
    pressureFieldComplexSerial pfcs(grid);
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

TEST(pressureFieldComplexSerialTest, operatorSubtractDoubleVectorTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(2.1, 3.1);
    pressureFieldComplexSerial pfcs(grid);
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

TEST(pressureFieldComplexSerialTest, operatorSubtractDoubleVectorTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue(2.1, 3.1);
    pressureFieldComplexSerial pfcs(grid);
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

TEST(pressureFieldComplexSerialTest, operatorSubtractComplexDoubleTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(31.0, 32.0);
    pressureFieldComplexSerial pfcs(grid);
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

TEST(pressureFieldComplexSerialTest, operatorSubtractDoubleTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(2.1, 3.1);
    pressureFieldComplexSerial pfcs(grid);
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

TEST(pressureFieldComplexSerialTest, operatorMultiplyByPressureFieldComplexSerialTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(2.1, 3.1);
    pressureFieldComplexSerial pfcs1(grid);
    pfcs1 = testValue1;

    std::complex<double> testValue2(3.1, 4.1);
    pressureFieldComplexSerial pfcs2(grid);
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

TEST(pressureFieldComplexSerialTest, operatorMultiplyByPressureFieldComplexSerialTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue(1.0, 2.0);
    pressureFieldComplexSerial pfcs1(grid);
    pfcs1 = testValue;

    std::complex<double> count(0.0, 0.0);
    std::complex<double> increment(1.0, 1.0);
    std::vector<std::complex<double>> dataIncreasing = std::vector<std::complex<double>>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count += increment;
    }

    pressureFieldComplexSerial pfcs2(grid);
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

TEST(pressureFieldComplexSerialTest, operatorMultiplyByPressureFieldSerialTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(1.0, 2.0);
    pressureFieldComplexSerial pfcs(grid);
    pfcs = testValue1;

    double testValue2 = 2.0;
    pressureFieldSerial pfs(grid);
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

TEST(pressureFieldComplexSerialTest, operatorMultiplyByPressureFieldSerialTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue(1.0, 2.0);
    pressureFieldComplexSerial pfcs(grid);
    pfcs = testValue;

    double count = 0;
    std::vector<double> dataIncreasing = std::vector<double>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count++;
    }

    pressureFieldSerial pfs(grid);
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

TEST(pressureFieldComplexSerialTest, operatorMultiplyByComplexDoubleVectorTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(2.0, 3.0);
    pressureFieldComplexSerial pfcs(grid);
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

TEST(pressureFieldComplexSerialTest, operatorMultiplyByComplexDoubleVectorTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue(21.0, 22.0);
    pressureFieldComplexSerial pfcs(grid);
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

TEST(pressureFieldComplexSerialTest, operatorMultiplyByDoubleVectorTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(2.1, 3.1);
    pressureFieldComplexSerial pfcs(grid);
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

TEST(pressureFieldComplexSerialTest, operatorMultiplyByDoubleVectorTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue(2.1, 3.1);
    pressureFieldComplexSerial pfcs(grid);
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

TEST(pressureFieldComplexSerialTest, operatorMultiplyByComplexDoubleTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(2.2, 3.3);
    pressureFieldComplexSerial pfcs(grid);
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

TEST(pressureFieldComplexSerialTest, operatorMultiplyByDoubleTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(2.1, 3.1);
    pressureFieldComplexSerial pfcs(grid);
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

TEST(pressureFieldComplexSerialTest, operatorDivideByPressureFieldComplexSerialExceptionTest)
{
    // Given
    grid2D grid = getGrid();

    std::complex<double> testValue1(3.7, 6.2);
    pressureFieldComplexSerial pfcs1(grid);
    pfcs1 = testValue1;

    pressureFieldComplexSerial pfcs2(grid);
    // Note: Initialized with zero

    // When
    // Nothing here, see below

    // Then
    EXPECT_THROW(pfcs1 /= pfcs2, std::overflow_error);
    // Note: 1 / 0.0 must throw
}

TEST(pressureFieldComplexSerialTest, operatorDivideByPressureFieldComplexSerialTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(-3.7, -2.7);
    pressureFieldComplexSerial pfcs1(grid);
    pfcs1 = testValue1;

    std::complex<double> testValue2(7.2, 3.9);
    pressureFieldComplexSerial pfcs2(grid);
    pfcs2 = testValue2;

    // When
    pfcs1 /= pfcs2;

    // Then
    std::complex<double> solution = testValue1 / testValue2;
    const std::vector<std::complex<double>> &data = pfcs1.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), solution.real());
        ASSERT_DOUBLE_EQ(data[i].imag(), solution.imag());
    }
}

TEST(pressureFieldComplexSerialTest, operatorDivideByPressureFieldSerialExceptionTest)
{
    // Given
    grid2D grid = getGrid();

    std::complex<double> testValue1(1.0, 1.0);
    pressureFieldComplexSerial pfcs(grid);
    pfcs = testValue1;

    pressureFieldSerial pfs(grid);
    // Note: Initialized with zero

    // When
    // Nothing here, see below

    // Then
    EXPECT_THROW(pfcs /= pfs, std::overflow_error);
    // Note: 1 / 0.0 must throw
}

TEST(pressureFieldComplexSerialTest, operatorDivideByPressureFieldSerialTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(3.7, 6.2);
    pressureFieldComplexSerial pfcs(grid);
    pfcs = testValue1;

    double testValue2 = 9.3;
    pressureFieldSerial pfs(grid);
    pfs = testValue2;

    // When
    pfcs /= pfs;

    // Then
    std::complex<double> solution = testValue1 / testValue2;
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), solution.real());
        ASSERT_DOUBLE_EQ(data[i].imag(), solution.imag());
    }
}

TEST(pressureFieldComplexSerialTest, operatorDivideByComplexDoubleVectorExceptionTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(1.0, 1.0);
    pressureFieldComplexSerial pfcs(grid);
    pfcs = testValue1;

    std::complex<double> zero(0.0, 0.0);
    auto dataVector = std::vector<std::complex<double>>(nrOfGridPoints, zero);
    // Note: Initialized with zero

    // When
    // Nothing here, see below

    // Then
    EXPECT_THROW(pfcs /= dataVector, std::overflow_error);
    // Note: 1 / 0.0 must throw
}

TEST(pressureFieldComplexSerialTest, operatorDivideByComplexDoubleVectorTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(3.8, 1.1);
    pressureFieldComplexSerial pfcs(grid);
    pfcs = testValue1;

    std::complex<double> testValue2(4.9, 3.6);
    auto dataVector = std::vector<std::complex<double>>(nrOfGridPoints, testValue2);

    // When
    pfcs /= dataVector;

    // Then
    std::complex<double> solution = testValue1 / testValue2;
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), solution.real());
        ASSERT_DOUBLE_EQ(data[i].imag(), solution.imag());
    }
}

TEST(pressureFieldComplexSerialTest, operatorDivideByComplexDoubleVectorTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue(6.7, -5.3);
    pressureFieldComplexSerial pfcs(grid);
    pfcs = testValue;

    double count = 1;   // Dont start at zero because of dividing by zero ...
    auto dataIncreasing = std::vector<std::complex<double>>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = std::complex<double>(count, count);
        count++;
    }

    // When
    pfcs /= dataIncreasing;

    // Then
    count = 1;   // Dont start at zero because of deviding by zero ...
    std::complex<double> solution;
    std::complex<double> value(0, 0);
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        value = std::complex<double>(count, count);
        solution = testValue / value;
        count++;
        ASSERT_DOUBLE_EQ(data[i].real(), solution.real());
        ASSERT_DOUBLE_EQ(data[i].imag(), solution.imag());
    }
}

TEST(pressureFieldComplexSerialTest, operatorDivideByDoubleVectorExceptionTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(1.0, 1.0);
    pressureFieldComplexSerial pfcs(grid);
    pfcs = testValue1;

    std::vector<double> dataVector = std::vector<double>(nrOfGridPoints, 0.0);
    // Note: Initialized with zero

    // When
    // Nothing here, see below

    // Then
    EXPECT_THROW(pfcs /= dataVector, std::overflow_error);
    // Note: 1 / 0.0 must throw
}

TEST(pressureFieldComplexSerialTest, operatorDivideByDoubleVectorTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(3.8, 1.1);
    pressureFieldComplexSerial pfcs(grid);
    pfcs = testValue1;

    double testValue2 = 22.4;
    std::vector<double> dataVector = std::vector<double>(nrOfGridPoints, testValue2);

    // When
    pfcs /= dataVector;

    // Then
    std::complex<double> solution = testValue1 / testValue2;
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), solution.real());
        ASSERT_DOUBLE_EQ(data[i].imag(), solution.imag());
    }
}

TEST(pressureFieldComplexSerialTest, operatorDivideByDoubleVectorTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue(6.7, -5.3);
    pressureFieldComplexSerial pfcs(grid);
    pfcs = testValue;

    double count = 1;   // Dont start at zero because of dividing by zero ...
    std::vector<double> dataIncreasing = std::vector<double>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count++;
    }

    // When
    pfcs /= dataIncreasing;

    // Then
    count = 1;   // Dont start at zero because of deviding by zero ...
    std::complex<double> solution;
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        solution = testValue / count;
        count++;
        ASSERT_DOUBLE_EQ(data[i].real(), solution.real());
        ASSERT_DOUBLE_EQ(data[i].imag(), solution.imag());
    }
}

TEST(pressureFieldComplexSerialTest, operatorDivideByComplexDoubleExceptionTest)
{
    // Given
    grid2D grid = getGrid();

    std::complex<double> testValue(1.0, 1.0);
    pressureFieldComplexSerial pfcs(grid);
    pfcs = testValue;

    std::complex<double> zero(0.0, 0.0);
    // When
    // Nothing here, see below

    // Then
    EXPECT_THROW(pfcs /= zero, std::overflow_error);
    // Note: 1 / 0.0 must throw
}

TEST(pressureFieldComplexSerialTest, operatorDivideByComplexDoubleTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(4.6, -0.7);
    pressureFieldComplexSerial pfcs(grid);
    pfcs = testValue1;

    // When
    std::complex<double> testValue2(6.1, 1.9);
    pfcs /= testValue2;

    // Then
    std::complex<double> solution = testValue1 / testValue2;
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), solution.real());
        ASSERT_DOUBLE_EQ(data[i].imag(), solution.imag());
    }
}

TEST(pressureFieldComplexSerialTest, operatorDivideByDoubleExceptionTest)
{
    // Given
    grid2D grid = getGrid();

    std::complex<double> testValue1(1.0, 1.0);
    pressureFieldComplexSerial pfcs(grid);
    pfcs = testValue1;

    // When
    // Nothing here, see below

    // Then
    EXPECT_THROW(pfcs /= 0.0, std::overflow_error);
    // Note: 1 / 0.0 must throw
}

TEST(pressureFieldComplexSerialTest, operatorDivideByDoubleTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(15.0, -1.9);
    pressureFieldComplexSerial pfcs(grid);
    pfcs = testValue1;

    // When
    double value2 = 2.33;
    pfcs /= value2;

    // Then
    std::complex<double> solution = testValue1 / value2;
    const std::vector<std::complex<double>> &data = pfcs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), solution.real());
        ASSERT_DOUBLE_EQ(data[i].imag(), solution.imag());
    }
}
