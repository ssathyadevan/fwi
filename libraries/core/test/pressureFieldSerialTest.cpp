#include "pressureFieldSerial.h"
#include <gtest/gtest.h>

Grid2D getGrid()
{
    std::array<double, 2> x_min = {0.0, 0.0};
    std::array<double, 2> x_max = {2.0, 2.0};
    std::array<int, 2> n_x = {2, 4};

    Grid2D grid(x_min, x_max, n_x);
    return grid;
}

// Virtual overrides
TEST(PressureFieldSerialTest, ZeroTest)
{
    // Given
    Grid2D grid = getGrid();
    PressureFieldSerial pfs(grid);
    pfs = 1.0;

    // When
    pfs.Zero();

    // Then
    const int nrOfGridPoints = pfs.GetNumberOfGridPoints();
    const std::vector<double> &data = pfs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], 0.0);
    }
}

TEST(PressureFieldSerialTest, SquareTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    double value = 5.0;
    PressureFieldSerial pfs(grid);
    pfs = value;

    // When
    pfs.Square();

    // Then
    double solution = value * value;
    const std::vector<double> &data = pfs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], solution);
    }
}

TEST(PressureFieldSerialTest, SqrtTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    double value = 36.0;
    PressureFieldSerial pfs(grid);
    pfs = value;

    // When
    pfs.Sqrt();

    // Then
    double solution = std::sqrt(value);
    const std::vector<double> &data = pfs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], solution);
    }
}

TEST(PressureFieldSerialTest, NormTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    double value = 1.0;
    PressureFieldSerial pfs(grid);
    pfs = value;

    // Alternative calculation method
    double innerProduct = 0.0;
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        innerProduct += value * value;
    }
    double sqrtInnerProduct = std::sqrt(innerProduct);

    // When
    double norm = pfs.Norm();

    // Then
    ASSERT_DOUBLE_EQ(norm, sqrtInnerProduct);
}

TEST(PressureFieldSerialTest, RelNormTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    double value = 2.0;
    PressureFieldSerial pfs(grid);
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
    double relNorm = pfs.RelNorm();

    // Then
    ASSERT_DOUBLE_EQ(relNorm, sqrtDividedInnerProduct);
}

TEST(PressureFieldSerialTest, ReciprocalTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    double value = 2.0;
    PressureFieldSerial pfs(grid);
    pfs = value;

    // When
    pfs.Reciprocal();

    // Then
    double solution = 1.0 / value;
    const std::vector<double> &data = pfs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], solution);
    }
}

TEST(PressureFieldSerialTest, ReciprocalExceptionTest)
{
    // Given
    Grid2D grid = getGrid();
    PressureFieldSerial pfs(grid);
    // Note: Initialized with zero

    // When
    // Nothing here

    // Then
    EXPECT_THROW(pfs.Reciprocal(), std::overflow_error);
    // Note: 1 / 0.0 must throw
}

TEST(PressureFieldSerialTest, SummationTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    double value = 2.0;
    PressureFieldSerial pfs(grid);
    pfs = value;

    // Alternative calculation method
    double summedValue = 0.0;
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        summedValue += value;
    }

    // When
    double summationResult = pfs.Summation();

    // Then
    ASSERT_DOUBLE_EQ(summationResult, summedValue);
}

// Non virtual members
TEST(PressureFieldSerialTest, SetDataTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    PressureFieldSerial pfs(grid);

    // Create vextor with increasing values
    int count = 0;
    std::vector<double> dataIncreasing = std::vector<double>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count++;
    }

    // When
    pfs.setData(dataIncreasing);

    // Then
    count = 0;
    const std::vector<double> &data = pfs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], count);
        count++;
    }
}

TEST(PressureFieldSerialTest, AddPositiveDataTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    double value = 11;
    PressureFieldSerial pfs(grid);
    pfs = value;

    // Create vextor with increasing values
    int count = 0;
    std::vector<double> dataIncreasing = std::vector<double>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count++;
    }

    // When
    pfs.addData(dataIncreasing);

    // Then
    count = 0;
    const std::vector<double> &data = pfs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], value + count);
        count++;
    }
}

TEST(PressureFieldSerialTest, AddNegativeDataTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    double value1 = 11;
    PressureFieldSerial pfs(grid);
    pfs = value1;

    // Create vextor with increasing values
    double value2 = -11;
    std::vector<double> negativeData = std::vector<double>(nrOfGridPoints, value2);

    // When
    pfs.addData(negativeData);

    // Then
    const std::vector<double> &data = pfs.getData();
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        ASSERT_DOUBLE_EQ(data[i], value1 + value2);
    }
}

TEST(PressureFieldSerialTest, AddValueAtIndexTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    double value = 13;
    int index = 3;
    PressureFieldSerial pfs(grid);

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

TEST(PressureFieldSerialTest, InnerProductTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    double value = 2.0;
    PressureFieldSerial pfs(grid);
    pfs = value;

    // Alternative calculation method
    double alternativeInnerProduct = 0.0;
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        alternativeInnerProduct += value * value;
    }

    // When
    double calculatedInerProduct = pfs.InnerProduct(pfs);

    // Then
    ASSERT_DOUBLE_EQ(calculatedInerProduct, alternativeInnerProduct);
}

TEST(PressureFieldSerialTest, GradientTest)
{
    // Given
    // Make grid with non-boundary cells, i.e. at least 3x3.
    const std::array<double, 2> x_min = {0.0, 0.0};
    const std::array<double, 2> x_max = {2.0, 2.0};
    const std::array<int, 2> n_x = {3, 4};
    Grid2D grid(x_min, x_max, n_x);
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();
    const std::array<double, 2> &dx = grid.GetCellDimensions();

    PressureFieldSerial pfs(grid);
    std::vector<PressureFieldSerial> pfs_out(2, PressureFieldSerial(grid));

    // Create data: Linear tilted plane, x & z are centroids of grid cell.
    std::vector<double> data = std::vector<double>(nrOfGridPoints, 0.0);
    const std::function<double(double, double)> func = [](double x, double z) { return x + z; };
    for(int i = 0; i < n_x[1]; i++)
    {
        double z = x_min[1] + (i + double(0.5)) * dx[1];
        for(int j = 0; j < n_x[0]; j++)
        {
            double x = x_min[0] + (j + double(0.5)) * dx[0];
            data[i * n_x[0] + j] = func(x, z);
        }
    }
    pfs.setData(data);

    // When
    pfs.Gradient(pfs_out);

    // Then
    const std::vector<double> &data0 = pfs_out[0].getData();
    EXPECT_NEAR(data0[0], 1, 0.01);
    EXPECT_NEAR(data0[11], 1, 0.01);

    const std::vector<double> &data1 = pfs_out[1].getData();
    EXPECT_NEAR(data1[0], 1, 0.01);
    EXPECT_NEAR(data1[11], 1, 0.01);
}

// Operators
TEST(PressureFieldSerialTest, OperatorAssignPressureFieldSerialExceptionTest)
{
    // Given
    Grid2D grid = getGrid();
    PressureFieldSerial pfs(grid);

    // When
    // Nothing here, see below

    // Then
    EXPECT_THROW(pfs = pfs, std::logic_error);
    // Note: 1 / 0.0 must throw
}

TEST(PressureFieldSerialTest, OperatorAssignPressureFieldSerialTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    double value = 5.0;
    PressureFieldSerial pfs1(grid);
    pfs1 = value;

    PressureFieldSerial pfs2(grid);
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

TEST(PressureFieldSerialTest, OperatorAssignDoubleTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();
    PressureFieldSerial pfs(grid);

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

TEST(PressureFieldSerialTest, OperatorAssignDoubleVectorTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();
    PressureFieldSerial pfs(grid);

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

TEST(PressureFieldSerialTest, OperatorAssignDoubleVectorTest2)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    double value = 2.0;
    PressureFieldSerial pfs(grid);
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

TEST(PressureFieldSerialTest, OperatorAddPressureFieldSerialTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    double value1 = 5.0;
    PressureFieldSerial pfs1(grid);
    pfs1 = value1;

    double value2 = 3.5;
    PressureFieldSerial pfs2(grid);
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

TEST(PressureFieldSerialTest, OperatorAddPressureFieldSerialTest2)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    double value = 5.0;
    PressureFieldSerial pfs1(grid);
    pfs1 = value;

    int count = 0;
    std::vector<double> dataIncreasing = std::vector<double>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count++;
    }

    PressureFieldSerial pfs2(grid);
    pfs2.setData(dataIncreasing);

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

TEST(PressureFieldSerialTest, OperatorAddDoubleVectorTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    double value1 = 5.0;
    PressureFieldSerial pfs(grid);
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

TEST(PressureFieldSerialTest, OperatorAddDoubleVectorTest2)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    double value = 5.0;
    PressureFieldSerial pfs(grid);
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

TEST(PressureFieldSerialTest, OperatorAddDoubleTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    double value1 = 5.0;
    PressureFieldSerial pfs(grid);
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

TEST(PressureFieldSerialTest, OperatorSubtractPressureFieldSerialTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    double value1 = 9.2;
    PressureFieldSerial pfs1(grid);
    pfs1 = value1;

    double value2 = 4.8;
    PressureFieldSerial pfs2(grid);
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

TEST(PressureFieldSerialTest, OperatorSubtractPressureFieldSerialTest2)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    double value = 5.0;
    PressureFieldSerial pfs1(grid);
    pfs1 = value;

    int count = 0;
    std::vector<double> dataIncreasing = std::vector<double>(nrOfGridPoints, 0.0);
    for(int i = 0; i < nrOfGridPoints; i++)
    {
        dataIncreasing[i] = count;
        count++;
    }

    PressureFieldSerial pfs2(grid);
    pfs2.setData(dataIncreasing);

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

TEST(PressureFieldSerialTest, OperatorSubtractDoubleVectorTest2)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    double value = 2.3;
    PressureFieldSerial pfs(grid);
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

TEST(PressureFieldSerialTest, OperatorSubtractDoubleVectorTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    double value1 = 3.8;
    PressureFieldSerial pfs(grid);
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

TEST(PressureFieldSerialTest, OperatorSubtractDoubleTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    double value1 = 15.0;
    PressureFieldSerial pfs(grid);
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

TEST(PressureFieldSerialTest, OperatorMultiplyByPressureFieldSerialTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    double value1 = 4.9;
    PressureFieldSerial pfs1(grid);
    pfs1 = value1;

    double value2 = 7.0;
    PressureFieldSerial pfs2(grid);
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

TEST(PressureFieldSerialTest, OperatorMultiplyByDoubleVectorTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    double value1 = 6.2;
    PressureFieldSerial pfs(grid);
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

TEST(PressureFieldSerialTest, OperatorMultiplyByDoubleVectorTest2)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    double value = 4.9;
    PressureFieldSerial pfs(grid);
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

TEST(PressureFieldSerialTest, OperatorMultiplyByDoubleTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    double value1 = 3.3;
    PressureFieldSerial pfs(grid);
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

TEST(PressureFieldSerialTest, OperatorDivideByPressureFieldSerialExceptionTest)
{
    // Given
    Grid2D grid = getGrid();

    PressureFieldSerial pfs1(grid);
    pfs1 = 1.0;

    PressureFieldSerial pfs2(grid);
    // Note: Initialized with zero

    // When
    // Nothing here, see below

    // Then
    EXPECT_THROW(pfs1 /= pfs2, std::overflow_error);
    // Note: 1 / 0.0 must throw
}

TEST(PressureFieldSerialTest, OperatorDivideByPressureFieldSerialTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    double value1 = 5.0;
    PressureFieldSerial pfs1(grid);
    pfs1 = value1;

    double value2 = 15.0;
    PressureFieldSerial pfs2(grid);
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

TEST(PressureFieldSerialTest, OperatorDivideByDoubleVectorExceptionTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    PressureFieldSerial pfs1(grid);
    pfs1 = 1.0;

    std::vector<double> dataVector = std::vector<double>(nrOfGridPoints, 0.0);
    // Note: Initialized with zero

    // When
    // Nothing here, see below

    // Then
    EXPECT_THROW(pfs1 /= dataVector, std::overflow_error);
    // Note: 1 / 0.0 must throw
}

TEST(PressureFieldSerialTest, OperatorDivideByDoubleVectorTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    double value1 = 3.8;
    PressureFieldSerial pfs(grid);
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

TEST(PressureFieldSerialTest, OperatorDivideByDoubleVectorTest2)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    double value = 4.9;
    PressureFieldSerial pfs(grid);
    pfs = value;

    int count = 1;   // Dont start at zero because of deviding by zero ...
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

TEST(PressureFieldSerialTest, OperatorDivideByDoubleExceptionTest)
{
    // Given
    Grid2D grid = getGrid();

    PressureFieldSerial pfs1(grid);
    pfs1 = 1.0;

    // When
    // Nothing here, see below

    // Then
    EXPECT_THROW(pfs1 /= 0.0, std::overflow_error);
    // Note: 1 / 0.0 must throw
}

TEST(PressureFieldSerialTest, OperatorDivideByDoubleTest)
{
    // Given
    Grid2D grid = getGrid();
    const int nrOfGridPoints = grid.GetNumberOfGridPoints();

    double value1 = 15.0;
    PressureFieldSerial pfs(grid);
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
