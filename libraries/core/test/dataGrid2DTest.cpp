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

// Virtual overrides
TEST(pressureFieldSerialTest, zeroTest)
{
    // Given
    grid2D grid = getGrid();
    dataGrid2D pfs(grid);
    pfs = 1.0;

    // When
    pfs.zero();

    // Then
    const int nrOfGridPoints = pfs.getNumberOfGridPoints();
    const std::vector<double> &data = pfs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], 0.0);
    }
}

TEST(pressureFieldSerialTest, squareTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value = 5.0;
    dataGrid2D pfs(grid);
    pfs = value;

    // When
    pfs.square();

    // Then
    double solution = value * value;
    const std::vector<double> &data = pfs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], solution);
    }
}

TEST(pressureFieldSerialTest, sqrtTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value = 36.0;
    dataGrid2D pfs(grid);
    pfs = value;

    // When
    pfs.sqrt();

    // Then
    double solution = std::sqrt(value);
    const std::vector<double> &data = pfs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], solution);
    }
}

TEST(pressureFieldSerialTest, normTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value = 1.0;
    dataGrid2D pfs(grid);
    pfs = value;

    // Alternative calculation method
    double innerProduct = 0.0;
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        innerProduct += value * value;
    }
    double sqrtInnerProduct = std::sqrt(innerProduct);

    // When
    double norm = pfs.norm();

    // Then
    ASSERT_DOUBLE_EQ(norm, sqrtInnerProduct);
}

TEST(pressureFieldSerialTest, relNormTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value = 2.0;
    dataGrid2D pfs(grid);
    pfs = value;

    // Alternative calculation method
    double innerProduct = 0.0;
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        innerProduct += value * value;
    }

    double dividedInnerProduct = innerProduct / nrOfGridPoints;
    double sqrtDividedInnerProduct = std::sqrt(dividedInnerProduct);

    // When
    double relNorm = pfs.relNorm();

    // Then
    ASSERT_DOUBLE_EQ(relNorm, sqrtDividedInnerProduct);
}

TEST(pressureFieldSerialTest, reciprocalTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value = 2.0;
    dataGrid2D pfs(grid);
    pfs = value;

    // When
    pfs.reciprocal();

    // Then
    double solution = 1.0 / value;
    const std::vector<double> &data = pfs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], solution);
    }
}

TEST(pressureFieldSerialTest, reciprocalExceptionTest)
{
    // Given
    grid2D grid = getGrid();
    dataGrid2D pfs(grid);
    // Note: Initialized with zero

    // When
    // Nothing here

    // Then
    EXPECT_THROW(pfs.reciprocal(), std::overflow_error);
    // Note: 1 / 0.0 must throw
}

TEST(pressureFieldSerialTest, summationTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value = 2.0;
    dataGrid2D pfs(grid);
    pfs = value;

    // Alternative calculation method
    double summedValue = 0.0;
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        summedValue += value;
    }

    // When
    double summationResult = pfs.summation();

    // Then
    ASSERT_DOUBLE_EQ(summationResult, summedValue);
}

// Non virtual members
TEST(pressureFieldSerialTest, addValueAtIndexTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value = 13;
    int index = 3;
    dataGrid2D pfs(grid);

    // When
    pfs.addValueAtIndex(value, index);

    // Then
    const std::vector<double> &data = pfs.getData();
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

TEST(pressureFieldSerialTest, innerProductTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value = 2.0;
    dataGrid2D pfs(grid);
    pfs = value;

    // Alternative calculation method
    double alternativeInnerProduct = 0.0;
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        alternativeInnerProduct += value * value;
    }

    // When
    double calculatedInnerProduct = pfs.innerProduct(pfs);

    // Then
    ASSERT_DOUBLE_EQ(calculatedInnerProduct, alternativeInnerProduct);
}

TEST(pressureFieldSerialTest, gradientTest)
{
    // Given
    // Make grid with non-boundary cells, i.e. at least 3x3.
    const std::array<double, 2> xMin = {0.0, 0.0};
    const std::array<double, 2> xMax = {2.0, 2.0};
    const std::array<int, 2> nX = {3, 4};
    grid2D grid(xMin, xMax, nX);
    const int nrOfGridPoints = grid.getNumberOfGridPoints();
    const std::array<double, 2> &dx = grid.getCellDimensions();

    dataGrid2D pfs(grid);
    std::vector<dataGrid2D> pfsOut(2, dataGrid2D(grid));

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
    pfs = data;

    // When
    pfs.gradient(pfsOut);

    // Then
    const std::vector<double> &data0 = pfsOut[0].getData();
    EXPECT_NEAR(data0[0], 1, 0.01);
    EXPECT_NEAR(data0[11], 1, 0.01);

    const std::vector<double> &data1 = pfsOut[1].getData();
    EXPECT_NEAR(data1[0], 1, 0.01);
    EXPECT_NEAR(data1[11], 1, 0.01);
}

// Operators
TEST(pressureFieldSerialTest, operatorAssignPressureFieldSerialExceptionTest)
{
    // Given
    grid2D grid = getGrid();
    dataGrid2D pfs(grid);

    // When
    // Nothing here, see below

    // Then
    EXPECT_THROW(pfs = pfs, std::logic_error);
    // Note: 1 / 0.0 must throw
}

TEST(pressureFieldSerialTest, operatorAssignPressureFieldSerialTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value = 5.0;
    dataGrid2D pfs1(grid);
    pfs1 = value;

    dataGrid2D pfs2(grid);
    // Note: Initialized with 0.0

    // When
    pfs2 = pfs1;

    // Then
    const std::vector<double> &pfs1Data = pfs1.getData();
    const std::vector<double> &pfs2Data = pfs2.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(pfs1Data[i], pfs2Data[i]);
        ASSERT_DOUBLE_EQ(pfs2Data[i], value);
    }
}

TEST(pressureFieldSerialTest, operatorAssignDoubleTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();
    dataGrid2D pfs(grid);

    double value = 5.0;

    // When
    pfs = value;

    // Then
    const std::vector<double> &data = pfs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], value);
    }
}

TEST(pressureFieldSerialTest, operatorAssignDoubleVectorTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();
    dataGrid2D pfs(grid);

    double value = 5.0;
    std::vector<double> dataVector = std::vector<double>(nrOfGridPoints, value);

    // When
    pfs = dataVector;

    // Then
    const std::vector<double> &data = pfs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], value);
    }
}

TEST(pressureFieldSerialTest, operatorAssignDoubleVectorTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value = 2.0;
    dataGrid2D pfs(grid);
    pfs = value;

    int count = 0;
    std::vector<double> dataIncreasing = std::vector<double>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count++;
    }

    // When
    pfs = dataIncreasing;

    // Then
    count = 0;
    const std::vector<double> &data = pfs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], count);
        count++;
    }
}

TEST(pressureFieldSerialTest, operatorAddPressureFieldSerialTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value1 = 5.0;
    dataGrid2D pfs1(grid);
    pfs1 = value1;

    double value2 = 3.5;
    dataGrid2D pfs2(grid);
    pfs2 = value2;

    // When
    pfs2 += pfs1;

    // Then
    double solution = value2 + value1;
    const std::vector<double> &data = pfs2.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], solution);
    }
}

TEST(pressureFieldSerialTest, operatorAddPressureFieldSerialTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value = 5.0;
    dataGrid2D pfs1(grid);
    pfs1 = value;

    int count = 0;
    std::vector<double> dataIncreasing = std::vector<double>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count++;
    }

    dataGrid2D pfs2(grid);
    pfs2 = dataIncreasing;

    // When
    pfs2 += pfs1;

    // Then
    count = 0;
    const std::vector<double> &data = pfs2.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], count + value);
        count++;
    }
}

TEST(pressureFieldSerialTest, operatorAddDoubleVectorTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value1 = 5.0;
    dataGrid2D pfs(grid);
    pfs = value1;

    double value2 = 8.3;
    std::vector<double> dataVector = std::vector<double>(nrOfGridPoints, value2);

    // When
    pfs += dataVector;

    // Then
    double solution = value1 + value2;
    const std::vector<double> &data = pfs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], solution);
    }
}

TEST(pressureFieldSerialTest, operatorAddDoubleVectorTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value = 5.0;
    dataGrid2D pfs(grid);
    pfs = value;

    int count = 0;
    std::vector<double> dataIncreasing = std::vector<double>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count++;
    }

    // When
    pfs += dataIncreasing;

    // Then
    count = 0;
    const std::vector<double> &data = pfs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], value + count);
        count++;
    }
}

TEST(pressureFieldSerialTest, operatorAddDoubleTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value1 = 5.0;
    dataGrid2D pfs(grid);
    pfs = value1;

    // When
    double value2 = 5.0;
    pfs += value2;

    // Then
    double solution = value1 + value2;
    const std::vector<double> &data = pfs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], solution);
    }
}

TEST(pressureFieldSerialTest, operatorSubtractPressureFieldSerialTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value1 = 9.2;
    dataGrid2D pfs1(grid);
    pfs1 = value1;

    double value2 = 4.8;
    dataGrid2D pfs2(grid);
    pfs2 = value2;

    // When
    pfs2 -= pfs1;

    // Then
    double solution = value2 - value1;
    const std::vector<double> &data = pfs2.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], solution);
    }
}

TEST(pressureFieldSerialTest, operatorSubtractPressureFieldSerialTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value = 5.0;
    dataGrid2D pfs1(grid);
    pfs1 = value;

    int count = 0;
    std::vector<double> dataIncreasing = std::vector<double>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count++;
    }

    dataGrid2D pfs2(grid);
    pfs2 = dataIncreasing;

    // When
    pfs2 -= pfs1;

    // Then
    count = 0;
    const std::vector<double> &data = pfs2.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], count - value);
        count++;
    }
}

TEST(pressureFieldSerialTest, operatorSubtractDoubleVectorTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value = 2.3;
    dataGrid2D pfs(grid);
    pfs = value;

    int count = 0;
    std::vector<double> dataIncreasing = std::vector<double>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count++;
    }

    // When
    pfs -= dataIncreasing;

    // Then
    count = 0;
    const std::vector<double> &data = pfs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], value - count);
        count++;
    }
}

TEST(pressureFieldSerialTest, operatorSubtractDoubleVectorTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value1 = 3.8;
    dataGrid2D pfs(grid);
    pfs = value1;

    double value2 = 22.4;
    std::vector<double> dataVector = std::vector<double>(nrOfGridPoints, value2);

    // When
    pfs -= dataVector;

    // Then
    double solution = value1 - value2;
    const std::vector<double> &data = pfs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], solution);
    }
}

TEST(pressureFieldSerialTest, operatorSubtractDoubleTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value1 = 15.0;
    dataGrid2D pfs(grid);
    pfs = value1;

    // When
    double value2 = 2.33;
    pfs -= value2;

    // Then
    double solution = value1 - value2;
    const std::vector<double> &data = pfs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], solution);
    }
}

TEST(pressureFieldSerialTest, operatorMultiplyByPressureFieldSerialTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value1 = 4.9;
    dataGrid2D pfs1(grid);
    pfs1 = value1;

    double value2 = 7.0;
    dataGrid2D pfs2(grid);
    pfs2 = value2;

    // When
    pfs2 *= pfs1;

    // Then
    double solution = value2 * value1;
    const std::vector<double> &data = pfs2.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], solution);
    }
}

TEST(pressureFieldSerialTest, operatorMultiplyByDoubleVectorTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value1 = 6.2;
    dataGrid2D pfs(grid);
    pfs = value1;

    double value2 = 1.3;
    std::vector<double> dataVector = std::vector<double>(nrOfGridPoints, value2);

    // When
    pfs *= dataVector;

    // Then
    double solution = value1 * value2;
    const std::vector<double> &data = pfs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], solution);
    }
}

TEST(pressureFieldSerialTest, operatorMultiplyByDoubleVectorTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value = 4.9;
    dataGrid2D pfs(grid);
    pfs = value;

    int count = 0;
    std::vector<double> dataIncreasing = std::vector<double>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count++;
    }

    // When
    pfs *= dataIncreasing;

    // Then
    count = 0;
    const std::vector<double> &data = pfs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], value * count);
        count++;
    }
}

TEST(pressureFieldSerialTest, operatorMultiplyByDoubleTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value1 = 3.3;
    dataGrid2D pfs(grid);
    pfs = value1;

    // When
    double value2 = 5.9;
    pfs *= value2;

    // Then
    double solution = value1 * value2;
    const std::vector<double> &data = pfs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], solution);
    }
}

TEST(pressureFieldSerialTest, operatorDivideByPressureFieldSerialExceptionTest)
{
    // Given
    grid2D grid = getGrid();

    dataGrid2D pfs1(grid);
    pfs1 = 1.0;

    dataGrid2D pfs2(grid);
    // Note: Initialized with zero

    // When
    // Nothing here, see below

    // Then
    EXPECT_THROW(pfs1 /= pfs2, std::overflow_error);
    // Note: 1 / 0.0 must throw
}

TEST(pressureFieldSerialTest, operatorDivideByPressureFieldSerialTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value1 = 5.0;
    dataGrid2D pfs1(grid);
    pfs1 = value1;

    double value2 = 15.0;
    dataGrid2D pfs2(grid);
    pfs2 = value2;

    // When
    pfs2 /= pfs1;

    // Then
    double solution = value2 / value1;
    const std::vector<double> &data = pfs2.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], solution);
    }
}

TEST(pressureFieldSerialTest, operatorDivideByDoubleVectorExceptionTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    dataGrid2D pfs(grid);
    pfs = 1.0;

    std::vector<double> dataVector = std::vector<double>(nrOfGridPoints, 0.0);
    // Note: Initialized with zero

    // When
    // Nothing here, see below

    // Then
    EXPECT_THROW(pfs /= dataVector, std::overflow_error);
    // Note: 1 / 0.0 must throw
}

TEST(pressureFieldSerialTest, operatorDivideByDoubleVectorTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value1 = 3.8;
    dataGrid2D pfs(grid);
    pfs = value1;

    double value2 = 22.4;
    std::vector<double> dataVector = std::vector<double>(nrOfGridPoints, value2);

    // When
    pfs /= dataVector;

    // Then
    double solution = value1 / value2;
    const std::vector<double> &data = pfs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], solution);
    }
}

TEST(pressureFieldSerialTest, operatorDivideByDoubleVectorTest2)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value = 4.9;
    dataGrid2D pfs(grid);
    pfs = value;

    int count = 1;   // Dont start at zero because of dividing by zero ...
    std::vector<double> dataIncreasing = std::vector<double>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count++;
    }

    // When
    pfs /= dataIncreasing;

    // Then
    count = 1;   // Dont start at zero because of deviding by zero ...
    const std::vector<double> &data = pfs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], value / count);
        count++;
    }
}

TEST(pressureFieldSerialTest, operatorDivideByDoubleExceptionTest)
{
    // Given
    grid2D grid = getGrid();

    dataGrid2D pfs1(grid);
    pfs1 = 1.0;

    // When
    // Nothing here, see below

    // Then
    EXPECT_THROW(pfs1 /= 0.0, std::overflow_error);
    // Note: 1 / 0.0 must throw
}

TEST(pressureFieldSerialTest, operatorDivideByDoubleTest)
{
    // Given
    grid2D grid = getGrid();
    const int nrOfGridPoints = grid.getNumberOfGridPoints();

    double value1 = 15.0;
    dataGrid2D pfs(grid);
    pfs = value1;

    // When
    double value2 = 2.33;
    pfs /= value2;

    // Then
    double solution = value1 / value2;
    const std::vector<double> &data = pfs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], solution);
    }
}
