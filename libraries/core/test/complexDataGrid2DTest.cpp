#include "complexDataGrid2D.h"
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

TEST(complexDataGrid2DTest, copyConstructorTest)
{
    // Given
    std::complex<double> testValue(3.0, 4.6);
    grid2D grid = getGrid();
    complexDataGrid2D cdg1(grid);
    cdg1 = testValue;

    // When
    complexDataGrid2D cdg2(cdg1);

    // Then
    const int nrOfGridPoints = cdg2.getNumberOfGridPoints();
    const std::vector<std::complex<double>> &data = cdg2.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testValue));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testValue));
    }
}

TEST(complexDataGrid2DTest, zeroTest)
{
    // Given
    grid2D grid = getGrid();

    std::complex<double> testValue(1.0, 1.0);
    complexDataGrid2D cdg(grid);
    cdg = testValue;

    // When
    cdg.zero();

    // Then
    const int nrOfGridPoints = cdg.getNumberOfGridPoints();
    const std::vector<std::complex<double>> &data = cdg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), 0.0);
        ASSERT_DOUBLE_EQ(data[i].imag(), 0.0);
    }
}

TEST(complexDataGrid2DTest, squareTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue(1.0, 2.0);
    complexDataGrid2D cdg(grid);
    cdg = testValue;

    // when
    cdg.square();

    // Then
    // std::complex<double> testSolution = sqrt(testValue);
    std::complex<double> testSolution = testValue * testValue;
    const std::vector<std::complex<double>> &data = cdg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testSolution));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testSolution));
    }
}

TEST(complexDataGrid2DTest, sqrtTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue(4.0, 9.0);
    complexDataGrid2D cdg(grid);
    cdg = testValue;

    // When
    cdg.sqrt();

    // Then
    std::complex<double> testSolution = std::sqrt(testValue);
    const std::vector<std::complex<double>> &data = cdg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testSolution));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testSolution));
    }
}

TEST(complexDataGrid2DTest, reciprocalTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue(1.0, 2.0);
    complexDataGrid2D cdg(grid);
    cdg = testValue;

    // When
    cdg.reciprocal();

    // Then
    std::complex<double> testSolution = 1.0 / testValue;
    const std::vector<std::complex<double>> &data = cdg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testSolution));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testSolution));
    }
}

TEST(complexDataGrid2DTest, reciprocalExceptionTest)
{
    // Given
    grid2D grid = getGrid();
    complexDataGrid2D cdg(grid);
    // Note: Initialized with zero

    // When
    // Nothing here

    // Then
    EXPECT_THROW(cdg.reciprocal(), std::overflow_error);
    // Note: 1 / 0.0 must throw
}

TEST(complexDataGrid2DTest, summationTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue = 2.0;
    complexDataGrid2D cdg(grid);
    cdg = testValue;

    // Alternative calculation method
    std::complex<double> summedValue = 0.0;
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        summedValue += testValue;
    }

    // When
    std::complex<double> summationResult = cdg.Summation();

    // Then
    ASSERT_DOUBLE_EQ(summationResult.real(), real(summedValue));
    ASSERT_DOUBLE_EQ(summationResult.imag(), imag(summedValue));
}

// Non virtual members
TEST(complexDataGrid2DTest, addValueAtIndexTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue = 13;
    int index = 3;
    complexDataGrid2D cdg(grid);

    // When
    cdg.addValueAtIndex(testValue, index);

    // Then
    const std::vector<std::complex<double>> &data = cdg.getData();
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

TEST(complexDataGrid2DTest, innerProductTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue = {2.0, 1.0};
    complexDataGrid2D cdg(grid);
    cdg = testValue;

    // Alternative calculation method
    double alternativeInnerProduct = 0.0;
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        alternativeInnerProduct += real(testValue) * real(testValue) + imag(testValue) * imag(testValue);
    }

    // When
    double calculatedInerProduct = cdg.innerProduct(cdg);

    // Then
    ASSERT_DOUBLE_EQ(calculatedInerProduct, alternativeInnerProduct);
}

// Operators
TEST(complexDataGrid2DTest, operatorAssignComplexDataGrid2DExceptionTest)
{
    // Given
    grid2D grid = getGrid();
    complexDataGrid2D cdg(grid);

    // When
    // Nothing here, see below

    // Then
    EXPECT_THROW(cdg = cdg, std::logic_error);
}

TEST(complexDataGrid2DTest, operatorAssignComplexDataGrid2DTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue(0.0, 0.0);
    complexDataGrid2D cdg1(grid);
    cdg1 = testValue;

    complexDataGrid2D cdg2(grid);
    // Note: Initialized with 0.0

    // When
    cdg2 = cdg1;

    // Then
    const std::vector<std::complex<double>> &cdg1Data = cdg1.getData();
    const std::vector<std::complex<double>> &cdg2Data = cdg2.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(cdg1Data[i].real(), cdg2Data[i].real());
        ASSERT_DOUBLE_EQ(cdg2Data[i].real(), real(testValue));
        ASSERT_DOUBLE_EQ(cdg1Data[i].imag(), cdg2Data[i].imag());
        ASSERT_DOUBLE_EQ(cdg2Data[i].imag(), imag(testValue));
    }
}

TEST(complexDataGrid2DTest, operatorAssignDataGrid2DTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue(0.0, 0.0);
    complexDataGrid2D cdg(grid);
    cdg = testValue;

    dataGrid2D dg(grid);
    // Note: Initialized with 0.0

    // When
    cdg = dg;

    // Then
    const std::vector<std::complex<double>> &cdgData = cdg.getData();
    const std::vector<double> &dgData = dg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(cdgData[i].real(), dgData[i]);
    }
}

TEST(complexDataGrid2DTest, operatorAssignComplexDoubleVectorTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();
    complexDataGrid2D cdg(grid);

    std::complex<double> testValue(1.0, 2.0);
    std::vector<std::complex<double>> dataVector = std::vector<std::complex<double>>(nrOfGridPoints, testValue);

    // When
    cdg = dataVector;

    // Then
    const std::vector<std::complex<double>> &data = cdg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testValue));
    }
}

TEST(complexDataGrid2DTest, operatorAssignComplexDoubleVectorTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue(1.0, 2.0);
    complexDataGrid2D cdg(grid);
    cdg = testValue;

    std::complex<double> count(0.0, 0.0);
    std::complex<double> increment(1.0, 1.0);
    std::vector<std::complex<double>> dataIncreasing = std::vector<std::complex<double>>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count += increment;
    }

    // When
    cdg = dataIncreasing;

    // Then
    count.real(0.0);
    count.imag(0.0);
    const std::vector<std::complex<double>> &data = cdg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(count));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(count));
        count += increment;
    }
}

TEST(complexDataGrid2DTest, operatorAssignDoubleVectorTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();
    complexDataGrid2D cdg(grid);

    double testValue = 1.0;
    std::vector<double> dataVector = std::vector<double>(nrOfGridPoints, testValue);
    // When
    cdg = dataVector;

    // Then
    const std::vector<std::complex<double>> &data = cdg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), testValue);
    }
}

TEST(complexDataGrid2DTest, operatorAssignDoubleVectorTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double testValue = 1.0;
    complexDataGrid2D cdg(grid);
    cdg = testValue;

    int count = 0;
    std::vector<double> dataIncreasing = std::vector<double>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count++;
    }

    // When
    cdg = dataIncreasing;

    // Then
    count = 0;
    const std::vector<std::complex<double>> &data = cdg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), count);
        count++;
    }
}

TEST(complexDataGrid2DTest, operatorAssignComplexDoubleTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();
    complexDataGrid2D cdg(grid);

    std::complex<double> testValue(1.0, 2.0);

    // When
    cdg = testValue;

    // Then
    const std::vector<std::complex<double>> &data = cdg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testValue));
    }
}

TEST(complexDataGrid2DTest, operatorAssignDoubleTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();
    complexDataGrid2D cdg(grid);

    double testValue = 1.0;

    // When
    cdg = testValue;

    // Then
    const std::vector<std::complex<double>> &data = cdg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), testValue);
    }
}

TEST(complexDataGrid2DTest, operatorAddComplexDataGrid2DTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(1.0, 2.0);
    complexDataGrid2D cdg1(grid);
    cdg1 = testValue1;

    std::complex<double> testValue2(2.0, 3.0);
    complexDataGrid2D cdg2(grid);
    cdg2 = testValue2;

    // When
    cdg2 += cdg1;

    // Then
    std::complex<double> testSolution = testValue2 + testValue1;
    const std::vector<std::complex<double>> &data = cdg2.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testSolution));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testSolution));
    }
}

TEST(complexDataGrid2DTest, operatorAddComplexDataGrid2DTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue(1.0, 2.0);
    complexDataGrid2D cdg1(grid);
    cdg1 = testValue;

    std::complex<double> count(0.0, 0.0);
    std::complex<double> increment(1.0, 1.0);
    std::vector<std::complex<double>> dataIncreasing = std::vector<std::complex<double>>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count += increment;
    }

    complexDataGrid2D cdg2(grid);
    cdg2 = dataIncreasing;

    // When
    cdg2 += cdg1;

    // Then
    count.real(0.0);
    count.imag(0.0);
    const std::vector<std::complex<double>> &data = cdg2.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(count + testValue));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(count + testValue));
        count += increment;
    }
}

TEST(complexDataGrid2DTest, operatorAddDataGrid2DTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(1.0, 2.0);
    complexDataGrid2D cdg(grid);
    cdg = testValue1;

    double testValue2 = 2.0;
    dataGrid2D dg(grid);
    dg = testValue2;

    // When
    cdg += dg;

    // Then
    std::complex<double> testSolution = testValue2 + testValue1;
    const std::vector<std::complex<double>> &data = cdg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testSolution));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testSolution));
    }
}

TEST(complexDataGrid2DTest, operatorAddDataGrid2DTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue(1.0, 2.0);
    complexDataGrid2D cdg(grid);
    cdg = testValue;

    double count = 0;
    std::vector<double> dataIncreasing = std::vector<double>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count++;
    }

    dataGrid2D dg(grid);
    dg = dataIncreasing;

    // When
    cdg += dg;

    // Then
    count = 0;
    const std::vector<std::complex<double>> &data = cdg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(count + testValue));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(count + testValue));
        count++;
    }
}

TEST(complexDataGrid2DTest, operatorAddComplexDoubleVectorTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(2.0, 3.0);
    complexDataGrid2D cdg(grid);
    cdg = testValue1;

    std::complex<double> testValue2(3.1, 4.2);
    std::vector<std::complex<double>> dataVector = std::vector<std::complex<double>>(nrOfGridPoints, testValue2);

    // When
    cdg += dataVector;

    // Then
    std::complex<double> testSolution = testValue1 + testValue2;
    const std::vector<std::complex<double>> &data = cdg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testSolution));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testSolution));
    }
}

TEST(complexDataGrid2DTest, operatorAddComplexDoubleVectorTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue(1.0, 2.0);
    complexDataGrid2D cdg(grid);
    cdg = testValue;

    std::complex<double> count(0.0, 0.0);
    std::complex<double> increment(1.0, 1.0);
    std::vector<std::complex<double>> dataIncreasing = std::vector<std::complex<double>>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count += increment;
    }

    // When
    cdg += dataIncreasing;

    // Then
    count.real(0.0);
    count.imag(0.0);
    const std::vector<std::complex<double>> &data = cdg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(count + testValue));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(count + testValue));
        count += increment;
    }
}

TEST(complexDataGrid2DTest, operatorAddDoubleVectorTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double testValue = 1.0;
    complexDataGrid2D cdg(grid);

    std::vector<double> dataVector = std::vector<double>(nrOfGridPoints, testValue);
    // When
    cdg += dataVector;

    // Then
    const std::vector<std::complex<double>> &data = cdg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), testValue);
    }
}

TEST(complexDataGrid2DTest, operatorAddDoubleVectorTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue(2.1, 3.1);
    complexDataGrid2D cdg(grid);
    cdg = testValue;

    int count = 0;
    std::vector<double> dataIncreasing = std::vector<double>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count++;
    }

    // When
    cdg += dataIncreasing;

    // Then
    count = 0;
    const std::vector<std::complex<double>> &data = cdg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), (real(testValue) + count));
        count++;
    }
}

TEST(complexDataGrid2DTest, operatorAddComplexDoubleTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(1.0, 2.0);
    complexDataGrid2D cdg(grid);
    cdg = testValue1;

    // When
    std::complex<double> testValue2(2.0, 3.0);
    cdg += testValue2;

    // Then
    const std::vector<std::complex<double>> &data = cdg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testValue1 + testValue2));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testValue1 + testValue2));
    }
}

TEST(complexDataGrid2DTest, operatorAddDoubleTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(2.1, 3.1);
    complexDataGrid2D cdg(grid);
    cdg = testValue1;

    // When
    std::complex<double> testValue2(2.2, 3.2);
    cdg += testValue2;

    // Then
    std::complex<double> testSolution = testValue1 + testValue2;
    const std::vector<std::complex<double>> &data = cdg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testSolution));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testSolution));
    }
}

TEST(complexDataGrid2DTest, operatorSubtractComplexDataGrid2DTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1 = 9.2;
    complexDataGrid2D cdg1(grid);
    cdg1 = testValue1;

    std::complex<double> testValue2 = 4.8;
    complexDataGrid2D cdg2(grid);
    cdg2 = testValue2;

    // When
    cdg2 -= cdg1;

    // Then
    std::complex<double> testSolution = testValue2 - testValue1;
    const std::vector<std::complex<double>> &data = cdg2.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testSolution));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testSolution));
    }
}

TEST(complexDataGrid2DTest, operatorSubtractComplexDataGrid2DTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue(21.0, 22.0);
    complexDataGrid2D cdg1(grid);
    cdg1 = testValue;

    std::complex<double> count(0.0, 0.0);
    std::complex<double> increment(1.0, 1.0);
    std::vector<std::complex<double>> dataIncreasing = std::vector<std::complex<double>>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count += increment;
    }

    complexDataGrid2D cdg2(grid);
    cdg2 = dataIncreasing;

    // When
    cdg1 -= cdg2;

    // Then
    count.real(0.0);
    count.imag(0.0);
    const std::vector<std::complex<double>> &data = cdg1.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testValue - count));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testValue - count));
        count += increment;
    }
}

TEST(complexDataGrid2DTest, operatorSubtractDataGrid2DTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(1.0, 2.0);
    complexDataGrid2D cdg(grid);
    cdg = testValue1;

    double testValue2 = 2.0;
    dataGrid2D dg(grid);
    dg = testValue2;

    // When
    cdg -= dg;

    // Then
    std::complex<double> testSolution = testValue1 - testValue2;
    const std::vector<std::complex<double>> &data = cdg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testSolution));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testSolution));
    }
}

TEST(complexDataGrid2DTest, operatorSubtractDataGrid2DTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue(1.0, 2.0);
    complexDataGrid2D cdg(grid);
    cdg = testValue;

    double count = 0;
    std::vector<double> dataIncreasing = std::vector<double>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count++;
    }

    dataGrid2D dg(grid);
    dg = dataIncreasing;

    // When
    cdg -= dg;

    // Then
    count = 0.0;
    const std::vector<std::complex<double>> &data = cdg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testValue - count));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testValue - count));
        count++;
    }
}

TEST(complexDataGrid2DTest, operatorSubtractComplexDoubleVectorTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(2.0, 3.0);
    complexDataGrid2D cdg(grid);
    cdg = testValue1;

    std::complex<double> testValue2(3.1, 4.2);
    std::vector<std::complex<double>> dataVector = std::vector<std::complex<double>>(nrOfGridPoints, testValue2);

    // When
    cdg -= dataVector;

    // Then
    std::complex<double> testSolution = testValue1 - testValue2;
    const std::vector<std::complex<double>> &data = cdg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testSolution));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testSolution));
    }
}

TEST(complexDataGrid2DTest, operatorSubtractComplexDoubleVectorTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue(21.0, 22.0);
    complexDataGrid2D cdg(grid);
    cdg = testValue;

    std::complex<double> count(0.0, 0.0);
    std::complex<double> increment(1.0, 1.0);
    std::vector<std::complex<double>> dataIncreasing = std::vector<std::complex<double>>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count += increment;
    }

    // When
    cdg -= dataIncreasing;

    // Then
    count.real(0.0);
    count.imag(0.0);
    const std::vector<std::complex<double>> &data = cdg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testValue - count));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testValue - count));
        count += increment;
    }
}

TEST(complexDataGrid2DTest, operatorSubtractDoubleVectorTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(2.1, 3.1);
    complexDataGrid2D cdg(grid);
    cdg = testValue1;

    std::complex<double> testValue2(3.1, 4.1);
    std::vector<std::complex<double>> dataVector = std::vector<std::complex<double>>(nrOfGridPoints, testValue2);

    // When
    cdg -= dataVector;

    // Then
    std::complex<double> testSolution = testValue1 - testValue2;
    const std::vector<std::complex<double>> &data = cdg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testSolution));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testSolution));
    }
}

TEST(complexDataGrid2DTest, operatorSubtractDoubleVectorTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue(2.1, 3.1);
    complexDataGrid2D cdg(grid);
    cdg = testValue;

    std::complex<double> count(0.0, 0.0);
    std::complex<double> increment(1.0, 1.0);
    std::vector<std::complex<double>> dataIncreasing = std::vector<std::complex<double>>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count += increment;
    }

    // When
    cdg -= dataIncreasing;

    // Then
    count.real(0.0);
    count.imag(0.0);
    const std::vector<std::complex<double>> &data = cdg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testValue - count));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testValue - count));
        count += increment;
    }
}

TEST(complexDataGrid2DTest, operatorSubtractComplexDoubleTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(31.0, 32.0);
    complexDataGrid2D cdg(grid);
    cdg = testValue1;

    // When
    std::complex<double> testValue2(2.0, 3.0);
    cdg -= testValue2;

    // Then
    const std::vector<std::complex<double>> &data = cdg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testValue1 - testValue2));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testValue1 - testValue2));
    }
}

TEST(complexDataGrid2DTest, operatorSubtractDoubleTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(2.1, 3.1);
    complexDataGrid2D cdg(grid);
    cdg = testValue1;

    // When
    std::complex<double> testValue2(3.1, 4.1);
    cdg -= testValue2;

    // Then
    std::complex<double> testSolution = testValue1 - testValue2;
    const std::vector<std::complex<double>> &data = cdg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testSolution));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testSolution));
    }
}

TEST(complexDataGrid2DTest, operatorMultiplyByComplexDataGrid2DTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(2.1, 3.1);
    complexDataGrid2D cdg1(grid);
    cdg1 = testValue1;

    std::complex<double> testValue2(3.1, 4.1);
    complexDataGrid2D cdg2(grid);
    cdg2 = testValue2;

    // When
    cdg2 *= cdg1;

    // Then
    std::complex<double> testSolution = testValue2 * testValue1;
    const std::vector<std::complex<double>> &data = cdg2.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testSolution));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testSolution));
    }
}

TEST(complexDataGrid2DTest, operatorMultiplyByComplexDataGrid2DTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue(1.0, 2.0);
    complexDataGrid2D cdg1(grid);
    cdg1 = testValue;

    std::complex<double> count(0.0, 0.0);
    std::complex<double> increment(1.0, 1.0);
    std::vector<std::complex<double>> dataIncreasing = std::vector<std::complex<double>>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count += increment;
    }

    complexDataGrid2D cdg2(grid);
    cdg2 = dataIncreasing;

    // When
    cdg1 *= cdg2;

    // Then
    count.real(0.0);
    count.imag(0.0);
    const std::vector<std::complex<double>> &data = cdg1.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(count * testValue));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(count * testValue));
        count += increment;
    }
}

TEST(complexDataGrid2DTest, operatorMultiplyByDataGrid2DTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(1.0, 2.0);
    complexDataGrid2D cdg(grid);
    cdg = testValue1;

    double testValue2 = 2.0;
    dataGrid2D dg(grid);
    dg = testValue2;

    // When
    cdg *= dg;

    // Then
    std::complex<double> testSolution = testValue2 * testValue1;
    const std::vector<std::complex<double>> &data = cdg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testSolution));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testSolution));
    }
}

TEST(complexDataGrid2DTest, operatorMultiplyByDataGrid2DTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue(1.0, 2.0);
    complexDataGrid2D cdg(grid);
    cdg = testValue;

    double count = 0;
    std::vector<double> dataIncreasing = std::vector<double>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count++;
    }

    dataGrid2D dg(grid);
    dg = dataIncreasing;

    // When
    cdg *= dg;

    // Then
    count = 0;
    const std::vector<std::complex<double>> &data = cdg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(count * testValue));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(count * testValue));
        count++;
    }
}

TEST(complexDataGrid2DTest, operatorMultiplyByComplexDoubleVectorTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(2.0, 3.0);
    complexDataGrid2D cdg(grid);
    cdg = testValue1;

    std::complex<double> testValue2(3.1, 4.2);
    std::vector<std::complex<double>> dataVector = std::vector<std::complex<double>>(nrOfGridPoints, testValue2);

    // When
    cdg *= dataVector;

    // Then
    std::complex<double> testSolution = testValue1 * testValue2;
    const std::vector<std::complex<double>> &data = cdg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testSolution));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testSolution));
    }
}

TEST(complexDataGrid2DTest, operatorMultiplyByComplexDoubleVectorTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue(21.0, 22.0);
    complexDataGrid2D cdg(grid);
    cdg = testValue;

    std::complex<double> count(0.0, 0.0);
    std::complex<double> increment(1.0, 1.0);
    std::vector<std::complex<double>> dataIncreasing = std::vector<std::complex<double>>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count += increment;
    }

    // When
    cdg *= dataIncreasing;

    // Then
    count.real(0.0);
    count.imag(0.0);
    const std::vector<std::complex<double>> &data = cdg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testValue * count));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testValue * count));
        count += increment;
    }
}

TEST(complexDataGrid2DTest, operatorMultiplyByDoubleVectorTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(2.1, 3.1);
    complexDataGrid2D cdg(grid);
    cdg = testValue1;

    std::complex<double> testValue2(3.1, 4.1);
    std::vector<std::complex<double>> dataVector = std::vector<std::complex<double>>(nrOfGridPoints, testValue2);

    // When
    cdg *= dataVector;

    // Then
    std::complex<double> testSolution = testValue1 * testValue2;
    const std::vector<std::complex<double>> &data = cdg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testSolution));
    }
}

TEST(complexDataGrid2DTest, operatorMultiplyByDoubleVectorTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue(2.1, 3.1);
    complexDataGrid2D cdg(grid);
    cdg = testValue;

    std::complex<double> count(0.0, 0.0);
    std::complex<double> increment(1.0, 1.0);
    std::vector<std::complex<double>> dataIncreasing = std::vector<std::complex<double>>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count += increment;
    }

    // When
    cdg *= dataIncreasing;

    // Then
    count.real(0.0);
    count.imag(0.0);
    const std::vector<std::complex<double>> &data = cdg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testValue * count));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testValue * count));
        count += increment;
    }
}

TEST(complexDataGrid2DTest, operatorMultiplyByComplexDoubleTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(2.2, 3.3);
    complexDataGrid2D cdg(grid);
    cdg = testValue1;

    // When
    std::complex<double> testValue2(2.0, 3.0);
    cdg *= testValue2;

    // Then
    const std::vector<std::complex<double>> &data = cdg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testValue1 * testValue2));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testValue1 * testValue2));
    }
}

TEST(complexDataGrid2DTest, operatorMultiplyByDoubleTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(2.1, 3.1);
    complexDataGrid2D cdg(grid);
    cdg = testValue1;

    // When
    std::complex<double> testValue2(3.1, 4.1);
    cdg *= testValue2;

    // Then
    std::complex<double> testSolution = testValue1 * testValue2;
    const std::vector<std::complex<double>> &data = cdg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), real(testSolution));
        ASSERT_DOUBLE_EQ(data[i].imag(), imag(testSolution));
    }
}

TEST(complexDataGrid2DTest, operatorDivideByPressureFieldComplexSerialExceptionTest)
{
    // Given
    grid2D grid = getGrid();

    std::complex<double> testValue1(3.7, 6.2);
    complexDataGrid2D cdg1(grid);
    cdg1 = testValue1;

    complexDataGrid2D cdg2(grid);
    // Note: Initialized with zero

    // When
    // Nothing here, see below

    // Then
    EXPECT_THROW(cdg1 /= cdg2, std::overflow_error);
    // Note: 1 / 0.0 must throw
}

TEST(complexDataGrid2DTest, operatorDivideByComplexDataGrid2DTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(-3.7, -2.7);
    complexDataGrid2D cdg1(grid);
    cdg1 = testValue1;

    std::complex<double> testValue2(7.2, 3.9);
    complexDataGrid2D cdg2(grid);
    cdg2 = testValue2;

    // When
    cdg1 /= cdg2;

    // Then
    std::complex<double> solution = testValue1 / testValue2;
    const std::vector<std::complex<double>> &data = cdg1.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), solution.real());
        ASSERT_DOUBLE_EQ(data[i].imag(), solution.imag());
    }
}

TEST(complexDataGrid2DTest, operatorDivideByDataGrid2DExceptionTest)
{
    // Given
    grid2D grid = getGrid();

    std::complex<double> testValue1(1.0, 1.0);
    complexDataGrid2D cdg(grid);
    cdg = testValue1;

    dataGrid2D dg(grid);
    // Note: Initialized with zero

    // When
    // Nothing here, see below

    // Then
    EXPECT_THROW(cdg /= dg, std::overflow_error);
    // Note: 1 / 0.0 must throw
}

TEST(complexDataGrid2DTest, operatorDivideByDataGrid2DTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(3.7, 6.2);
    complexDataGrid2D cdg(grid);
    cdg = testValue1;

    double testValue2 = 9.3;
    dataGrid2D dg(grid);
    dg = testValue2;

    // When
    cdg /= dg;

    // Then
    std::complex<double> solution = testValue1 / testValue2;
    const std::vector<std::complex<double>> &data = cdg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), solution.real());
        ASSERT_DOUBLE_EQ(data[i].imag(), solution.imag());
    }
}

TEST(complexDataGrid2DTest, operatorDivideByComplexDoubleVectorExceptionTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(1.0, 1.0);
    complexDataGrid2D cdg(grid);
    cdg = testValue1;

    std::complex<double> zero(0.0, 0.0);
    auto dataVector = std::vector<std::complex<double>>(nrOfGridPoints, zero);
    // Note: Initialized with zero

    // When
    // Nothing here, see below

    // Then
    EXPECT_THROW(cdg /= dataVector, std::overflow_error);
    // Note: 1 / 0.0 must throw
}

TEST(complexDataGrid2DTest, operatorDivideByComplexDoubleVectorTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(3.8, 1.1);
    complexDataGrid2D cdg(grid);
    cdg = testValue1;

    std::complex<double> testValue2(4.9, 3.6);
    auto dataVector = std::vector<std::complex<double>>(nrOfGridPoints, testValue2);

    // When
    cdg /= dataVector;

    // Then
    std::complex<double> solution = testValue1 / testValue2;
    const std::vector<std::complex<double>> &data = cdg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), solution.real());
        ASSERT_DOUBLE_EQ(data[i].imag(), solution.imag());
    }
}

TEST(complexDataGrid2DTest, operatorDivideByComplexDoubleVectorTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue(6.7, -5.3);
    complexDataGrid2D cdg(grid);
    cdg = testValue;

    double count = 1;   // Dont start at zero because of dividing by zero ...
    auto dataIncreasing = std::vector<std::complex<double>>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = std::complex<double>(count, count);
        count++;
    }

    // When
    cdg /= dataIncreasing;

    // Then
    count = 1;   // Dont start at zero because of deviding by zero ...
    std::complex<double> solution;
    std::complex<double> value(0, 0);
    const std::vector<std::complex<double>> &data = cdg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        value = std::complex<double>(count, count);
        solution = testValue / value;
        count++;
        ASSERT_DOUBLE_EQ(data[i].real(), solution.real());
        ASSERT_DOUBLE_EQ(data[i].imag(), solution.imag());
    }
}

TEST(complexDataGrid2DTest, operatorDivideByDoubleVectorExceptionTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(1.0, 1.0);
    complexDataGrid2D cdg(grid);
    cdg = testValue1;

    std::vector<double> dataVector = std::vector<double>(nrOfGridPoints, 0.0);
    // Note: Initialized with zero

    // When
    // Nothing here, see below

    // Then
    EXPECT_THROW(cdg /= dataVector, std::overflow_error);
    // Note: 1 / 0.0 must throw
}

TEST(complexDataGrid2DTest, operatorDivideByDoubleVectorTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(3.8, 1.1);
    complexDataGrid2D cdg(grid);
    cdg = testValue1;

    double testValue2 = 22.4;
    std::vector<double> dataVector = std::vector<double>(nrOfGridPoints, testValue2);

    // When
    cdg /= dataVector;

    // Then
    std::complex<double> solution = testValue1 / testValue2;
    const std::vector<std::complex<double>> &data = cdg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), solution.real());
        ASSERT_DOUBLE_EQ(data[i].imag(), solution.imag());
    }
}

TEST(complexDataGrid2DTest, operatorDivideByDoubleVectorTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue(6.7, -5.3);
    complexDataGrid2D cdg(grid);
    cdg = testValue;

    double count = 1;   // Dont start at zero because of dividing by zero ...
    std::vector<double> dataIncreasing = std::vector<double>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count++;
    }

    // When
    cdg /= dataIncreasing;

    // Then
    count = 1;   // Dont start at zero because of deviding by zero ...
    std::complex<double> solution;
    const std::vector<std::complex<double>> &data = cdg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        solution = testValue / count;
        count++;
        ASSERT_DOUBLE_EQ(data[i].real(), solution.real());
        ASSERT_DOUBLE_EQ(data[i].imag(), solution.imag());
    }
}

TEST(complexDataGrid2DTest, operatorDivideByComplexDoubleExceptionTest)
{
    // Given
    grid2D grid = getGrid();

    std::complex<double> testValue(1.0, 1.0);
    complexDataGrid2D cdg(grid);
    cdg = testValue;

    std::complex<double> zero(0.0, 0.0);
    // When
    // Nothing here, see below

    // Then
    EXPECT_THROW(cdg /= zero, std::overflow_error);
    // Note: 1 / 0.0 must throw
}

TEST(complexDataGrid2DTest, operatorDivideByComplexDoubleTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(4.6, -0.7);
    complexDataGrid2D cdg(grid);
    cdg = testValue1;

    // When
    std::complex<double> testValue2(6.1, 1.9);
    cdg /= testValue2;

    // Then
    std::complex<double> solution = testValue1 / testValue2;
    const std::vector<std::complex<double>> &data = cdg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), solution.real());
        ASSERT_DOUBLE_EQ(data[i].imag(), solution.imag());
    }
}

TEST(complexDataGrid2DTest, operatorDivideByDoubleExceptionTest)
{
    // Given
    grid2D grid = getGrid();

    std::complex<double> testValue1(1.0, 1.0);
    complexDataGrid2D cdg(grid);
    cdg = testValue1;

    // When
    // Nothing here, see below

    // Then
    EXPECT_THROW(cdg /= 0.0, std::overflow_error);
    // Note: 1 / 0.0 must throw
}

TEST(complexDataGrid2DTest, operatorDivideByDoubleTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    std::complex<double> testValue1(15.0, -1.9);
    complexDataGrid2D cdg(grid);
    cdg = testValue1;

    // When
    double value2 = 2.33;
    cdg /= value2;

    // Then
    std::complex<double> solution = testValue1 / value2;
    const std::vector<std::complex<double>> &data = cdg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i].real(), solution.real());
        ASSERT_DOUBLE_EQ(data[i].imag(), solution.imag());
    }
}
