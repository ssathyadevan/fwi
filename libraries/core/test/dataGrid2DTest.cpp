#include "dataGrid2D.h"
#include <gtest/gtest.h>

grid2D getGrid()
{
    std::array<double, 2> xMin = {0.0, 0.0};
    std::array<double, 2> xMax = {2.0, 2.0};
    std::array<int, 2> nX = {2, 4};

    grid2D grid(xMin, xMax, nX);
    return grid;
}

TEST(dataGrid2DTest, copyConstructorTest)
{
    // Given
    double testValue = 3.0;
    grid2D grid = getGrid();
    dataGrid2D dg1(grid);
    dg1 = testValue;

    // When
    dataGrid2D dg2(dg1);

    // Then
    const int nrOfGridPoints = dg2.getNumberOfGridPoints();
    const std::vector<double> &data = dg2.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], testValue);
    }
}

TEST(dataGrid2DTest, squareTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value = 5.0;
    dataGrid2D dg(grid);
    dg = value;

    // When
    dg.square();

    // Then
    double solution = value * value;
    const std::vector<double> &data = dg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], solution);
    }
}

TEST(dataGrid2DTest, sqrtTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value = 36.0;
    dataGrid2D dg(grid);
    dg = value;

    // When
    dg.sqrt();

    // Then
    double solution = std::sqrt(value);
    const std::vector<double> &data = dg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], solution);
    }
}

TEST(dataGrid2DTest, normTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value = 1.0;
    dataGrid2D dg(grid);
    dg = value;

    // Alternative calculation method
    double innerProduct = 0.0;
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        innerProduct += value * value;
    }
    double sqrtInnerProduct = std::sqrt(innerProduct);

    // When
    double norm = dg.norm();

    // Then
    ASSERT_DOUBLE_EQ(norm, sqrtInnerProduct);
}

TEST(dataGrid2DTest, relNormTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value = 2.0;
    dataGrid2D dg(grid);
    dg = value;

    // Alternative calculation method
    double innerProduct = 0.0;
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        innerProduct += value * value;
    }

    double dividedInnerProduct = innerProduct / nrOfGridPoints;
    double sqrtDividedInnerProduct = std::sqrt(dividedInnerProduct);

    // When
    double relNorm = dg.relNorm();

    // Then
    ASSERT_DOUBLE_EQ(relNorm, sqrtDividedInnerProduct);
}

TEST(dataGrid2DTest, reciprocalTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value = 2.0;
    dataGrid2D dg(grid);
    dg = value;

    // When
    dg.reciprocal();

    // Then
    double solution = 1.0 / value;
    const std::vector<double> &data = dg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], solution);
    }
}

TEST(dataGrid2DTest, reciprocalExceptionTest)
{
    // Given
    grid2D grid = getGrid();
    dataGrid2D dg(grid);
    // Note: Initialized with zero

    // When
    // Nothing here

    // Then
    EXPECT_THROW(dg.reciprocal(), std::overflow_error);
    // Note: 1 / 0.0 must throw
}

TEST(dataGrid2DTest, summationTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value = 2.0;
    dataGrid2D dg(grid);
    dg = value;

    // Alternative calculation method
    double summedValue = 0.0;
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        summedValue += value;
    }

    // When
    double summationResult = dg.summation();

    // Then
    ASSERT_DOUBLE_EQ(summationResult, summedValue);
}

// Non virtual members
TEST(dataGrid2DTest, addValueAtIndexTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value = 13;
    int index = 3;
    dataGrid2D dg(grid);

    // When
    dg.addValueAtIndex(value, index);

    // Then
    const std::vector<double> &data = dg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        if(i != index)
        {
            ASSERT_DOUBLE_EQ(data[i], 0);
        }
        else
        {
            ASSERT_DOUBLE_EQ(data[i], value);
        }
    }
}

TEST(dataGrid2DTest, innerProductTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value = 2.0;
    dataGrid2D dg(grid);
    dg = value;

    // Alternative calculation method
    double alternativeInnerProduct = 0.0;
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        alternativeInnerProduct += value * value;
    }

    // When
    double calculatedInnerProduct = dg.innerProduct(dg);

    // Then
    ASSERT_DOUBLE_EQ(calculatedInnerProduct, alternativeInnerProduct);
}

TEST(dataGrid2DTest, gradientTest)
{
    // Given
    // Make grid with non-boundary cells, i.e. at least 3x3.
    const std::array<double, 2> xMin = {0.0, 0.0};
    const std::array<double, 2> xMax = {2.0, 2.0};
    const std::array<int, 2> nX = {3, 4};
    grid2D grid(xMin, xMax, nX);
    const int nrOfGridPoints = grid.getNumberOfGridPoints();
    const std::array<double, 2> &dx = grid.getCellDimensions();

    dataGrid2D dg(grid);
    std::vector<dataGrid2D> dgOut(2, dataGrid2D(grid));

    // Create data: Linear tilted plane, x & z are centroids of grid cell.
    std::vector<double> data = std::vector<double>(nrOfGridPoints, 0.0);
    const std::function<double(double, double)> func = [](double x, double z) { return x + z; };
    for(int i = 0; i < nX[1]; i++)
    {
        double z = xMin[1] + (i + double(0.5)) * dx[1];
        for(int j = 0; j < nX[0]; j++)
        {
            double x = xMin[0] + (j + double(0.5)) * dx[0];
            data[i * nX[0] + j] = func(x, z);
        }
    }
    dg = data;

    // When
    dg.gradient(dgOut);

    // Then
    const std::vector<double> &data0 = dgOut[0].getData();
    EXPECT_NEAR(data0[0], 1, 0.01);
    EXPECT_NEAR(data0[11], 1, 0.01);

    const std::vector<double> &data1 = dgOut[1].getData();
    EXPECT_NEAR(data1[0], 1, 0.01);
    EXPECT_NEAR(data1[11], 1, 0.01);
}

// Operators
TEST(dataGrid2DTest, operatorAssignPressureFieldSerialExceptionTest)
{
    // Given
    grid2D grid = getGrid();
    dataGrid2D dg(grid);

    // When
    // Nothing here, see below

    // Then
    EXPECT_THROW(dg = dg, std::logic_error);
    // Note: 1 / 0.0 must throw
}

TEST(dataGrid2DTest, operatorAssigndataGrid2DTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value = 5.0;
    dataGrid2D dg1(grid);
    dg1 = value;

    dataGrid2D dg2(grid);
    // Note: Initialized with 0.0

    // When
    dg2 = dg1;

    // Then
    const std::vector<double> &dg1Data = dg1.getData();
    const std::vector<double> &dg2Data = dg2.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(dg1Data[i], dg2Data[i]);
        ASSERT_DOUBLE_EQ(dg2Data[i], value);
    }
}

TEST(dataGrid2DTest, operatorAssignDoubleTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();
    dataGrid2D dg(grid);

    double value = 5.0;

    // When
    dg = value;

    // Then
    const std::vector<double> &data = dg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], value);
    }
}

TEST(dataGrid2DTest, operatorAssignDoubleVectorTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();
    dataGrid2D dg(grid);

    double value = 5.0;
    std::vector<double> dataVector = std::vector<double>(nrOfGridPoints, value);

    // When
    dg = dataVector;

    // Then
    const std::vector<double> &data = dg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], value);
    }
}

TEST(dataGrid2DTest, operatorAssignDoubleVectorTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value = 2.0;
    dataGrid2D dg(grid);
    dg = value;

    int count = 0;
    std::vector<double> dataIncreasing = std::vector<double>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count++;
    }

    // When
    dg = dataIncreasing;

    // Then
    count = 0;
    const std::vector<double> &data = dg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], count);
        count++;
    }
}

TEST(dataGrid2DTest, operatorAdddataGrid2DTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value1 = 5.0;
    dataGrid2D dg1(grid);
    dg1 = value1;

    double value2 = 3.5;
    dataGrid2D dg2(grid);
    dg2 = value2;

    // When
    dg2 += dg1;

    // Then
    double solution = value2 + value1;
    const std::vector<double> &data = dg2.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], solution);
    }
}

TEST(dataGrid2DTest, operatorAdddataGrid2DTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value = 5.0;
    dataGrid2D dg1(grid);
    dg1 = value;

    int count = 0;
    std::vector<double> dataIncreasing = std::vector<double>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count++;
    }

    dataGrid2D dg2(grid);
    dg2 = dataIncreasing;

    // When
    dg2 += dg1;

    // Then
    count = 0;
    const std::vector<double> &data = dg2.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], count + value);
        count++;
    }
}

TEST(dataGrid2DTest, operatorAddDoubleVectorTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value1 = 5.0;
    dataGrid2D dg(grid);
    dg = value1;

    double value2 = 8.3;
    std::vector<double> dataVector = std::vector<double>(nrOfGridPoints, value2);

    // When
    dg += dataVector;

    // Then
    double solution = value1 + value2;
    const std::vector<double> &data = dg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], solution);
    }
}

TEST(dataGrid2DTest, operatorAddDoubleVectorTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value = 5.0;
    dataGrid2D dg(grid);
    dg = value;

    int count = 0;
    std::vector<double> dataIncreasing = std::vector<double>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count++;
    }

    // When
    dg += dataIncreasing;

    // Then
    count = 0;
    const std::vector<double> &data = dg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], value + count);
        count++;
    }
}

TEST(dataGrid2DTest, operatorAddDoubleTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value1 = 5.0;
    dataGrid2D dg(grid);
    dg = value1;

    // When
    double value2 = 5.0;
    dg += value2;

    // Then
    double solution = value1 + value2;
    const std::vector<double> &data = dg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], solution);
    }
}

TEST(dataGrid2DTest, operatorSubtractdataGrid2DTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value1 = 9.2;
    dataGrid2D dg1(grid);
    dg1 = value1;

    double value2 = 4.8;
    dataGrid2D dg2(grid);
    dg2 = value2;

    // When
    dg2 -= dg1;

    // Then
    double solution = value2 - value1;
    const std::vector<double> &data = dg2.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], solution);
    }
}

TEST(dataGrid2DTest, operatorSubtractdataGrid2DTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value = 5.0;
    dataGrid2D dg1(grid);
    dg1 = value;

    int count = 0;
    std::vector<double> dataIncreasing = std::vector<double>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count++;
    }

    dataGrid2D dg2(grid);
    dg2 = dataIncreasing;

    // When
    dg2 -= dg1;

    // Then
    count = 0;
    const std::vector<double> &data = dg2.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], count - value);
        count++;
    }
}

TEST(dataGrid2DTest, operatorSubtractDoubleVectorTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value = 2.3;
    dataGrid2D dg(grid);
    dg = value;

    int count = 0;
    std::vector<double> dataIncreasing = std::vector<double>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count++;
    }

    // When
    dg -= dataIncreasing;

    // Then
    count = 0;
    const std::vector<double> &data = dg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], value - count);
        count++;
    }
}

TEST(dataGrid2DTest, operatorSubtractDoubleVectorTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value1 = 3.8;
    dataGrid2D dg(grid);
    dg = value1;

    double value2 = 22.4;
    std::vector<double> dataVector = std::vector<double>(nrOfGridPoints, value2);

    // When
    dg -= dataVector;

    // Then
    double solution = value1 - value2;
    const std::vector<double> &data = dg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], solution);
    }
}

TEST(dataGrid2DTest, operatorSubtractDoubleTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value1 = 15.0;
    dataGrid2D dg(grid);
    dg = value1;

    // When
    double value2 = 2.33;
    dg -= value2;

    // Then
    double solution = value1 - value2;
    const std::vector<double> &data = dg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], solution);
    }
}

TEST(dataGrid2DTest, operatorMultiplyBydataGrid2DTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value1 = 4.9;
    dataGrid2D dg1(grid);
    dg1 = value1;

    double value2 = 7.0;
    dataGrid2D dg2(grid);
    dg2 = value2;

    // When
    dg2 *= dg1;

    // Then
    double solution = value2 * value1;
    const std::vector<double> &data = dg2.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], solution);
    }
}

TEST(dataGrid2DTest, operatorMultiplyByDoubleVectorTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value1 = 6.2;
    dataGrid2D dg(grid);
    dg = value1;

    double value2 = 1.3;
    std::vector<double> dataVector = std::vector<double>(nrOfGridPoints, value2);

    // When
    dg *= dataVector;

    // Then
    double solution = value1 * value2;
    const std::vector<double> &data = dg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], solution);
    }
}

TEST(dataGrid2DTest, operatorMultiplyByDoubleVectorTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value = 4.9;
    dataGrid2D dg(grid);
    dg = value;

    int count = 0;
    std::vector<double> dataIncreasing = std::vector<double>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count++;
    }

    // When
    dg *= dataIncreasing;

    // Then
    count = 0;
    const std::vector<double> &data = dg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], value * count);
        count++;
    }
}

TEST(dataGrid2DTest, operatorMultiplyByDoubleTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value1 = 3.3;
    dataGrid2D dg(grid);
    dg = value1;

    // When
    double value2 = 5.9;
    dg *= value2;

    // Then
    double solution = value1 * value2;
    const std::vector<double> &data = dg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], solution);
    }
}

TEST(dataGrid2DTest, operatorDivideByPressureFieldSerialExceptionTest)
{
    // Given
    grid2D grid = getGrid();

    dataGrid2D dg1(grid);
    dg1 = 1.0;

    dataGrid2D dg2(grid);
    // Note: Initialized with zero

    // When
    // Nothing here, see below

    // Then
    EXPECT_THROW(dg1 /= dg2, std::overflow_error);
    // Note: 1 / 0.0 must throw
}

TEST(dataGrid2DTest, operatorDivideBydataGrid2DTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value1 = 5.0;
    dataGrid2D dg1(grid);
    dg1 = value1;

    double value2 = 15.0;
    dataGrid2D dg2(grid);
    dg2 = value2;

    // When
    dg2 /= dg1;

    // Then
    double solution = value2 / value1;
    const std::vector<double> &data = dg2.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], solution);
    }
}

TEST(dataGrid2DTest, operatorDivideByDoubleVectorExceptionTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    dataGrid2D dg(grid);
    dg = 1.0;

    std::vector<double> dataVector = std::vector<double>(nrOfGridPoints, 0.0);
    // Note: Initialized with zero

    // When
    // Nothing here, see below

    // Then
    EXPECT_THROW(dg /= dataVector, std::overflow_error);
    // Note: 1 / 0.0 must throw
}

TEST(dataGrid2DTest, operatorDivideByDoubleVectorTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value1 = 3.8;
    dataGrid2D dg(grid);
    dg = value1;

    double value2 = 22.4;
    std::vector<double> dataVector = std::vector<double>(nrOfGridPoints, value2);

    // When
    dg /= dataVector;

    // Then
    double solution = value1 / value2;
    const std::vector<double> &data = dg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], solution);
    }
}

TEST(dataGrid2DTest, operatorDivideByDoubleVectorTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value = 4.9;
    dataGrid2D dg(grid);
    dg = value;

    int count = 1;   // Dont start at zero because of dividing by zero ...
    std::vector<double> dataIncreasing = std::vector<double>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count++;
    }

    // When
    dg /= dataIncreasing;

    // Then
    count = 1;   // Dont start at zero because of deviding by zero ...
    const std::vector<double> &data = dg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], value / count);
        count++;
    }
}

TEST(dataGrid2DTest, operatorDivideByDoubleExceptionTest)
{
    // Given
    grid2D grid = getGrid();

    dataGrid2D dg1(grid);
    dg1 = 1.0;

    // When
    // Nothing here, see below

    // Then
    EXPECT_THROW(dg1 /= 0.0, std::overflow_error);
    // Note: 1 / 0.0 must throw
}

TEST(dataGrid2DTest, operatorDivideByDoubleTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value1 = 15.0;
    dataGrid2D dg(grid);
    dg = value1;

    // When
    double value2 = 2.33;
    dg /= value2;

    // Then
    double solution = value1 / value2;
    const std::vector<double> &data = dg.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], solution);
    }
}
