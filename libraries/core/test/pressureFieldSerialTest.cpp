#include <gtest/gtest.h>
#include "pressureFieldSerial.h"

Grid2D getGrid()
{
    std::array<double, 2> x_min = {0.0, 0.0};
    std::array<double, 2> x_max = {2.0, 2.0};
    std::array<int, 2> n_x = {2, 4};

    Grid2D grid(x_min, x_max, n_x);
    return grid;
}

PressureFieldSerial getPFS()
{
    Grid2D grid = getGrid();
    PressureFieldSerial pfs(grid);
    std::function<double(double, double)> func = [](double x, double z) { return x + z; }; // Linear tilted plane, x & z are centroids of grid cell.
    pfs.SetField(func);
    return pfs;
}

TEST(PressureFieldSerialTest, pfsNormTest)
{
    PressureFieldSerial pfs = getPFS();
    EXPECT_NEAR(pfs.Norm(), 6.04, 0.01);
}
TEST(PressureFieldSerialTest, pfsRelNormTest)
{
    PressureFieldSerial pfs = getPFS();
    EXPECT_NEAR(pfs.RelNorm(), 2.14, 0.01);
}

TEST(PressureFieldSerialTest, SquareTest)
{
    PressureFieldSerial pfs = getPFS();
    pfs.Square();
    double *ptr = pfs.GetDataPtr();
    EXPECT_NEAR(ptr[7], 10.5625, 0.001);
}

TEST(PressureFieldSerialTest, SqrtTest)
{
    PressureFieldSerial pfs = getPFS();
    pfs.Sqrt();
    double *ptr = pfs.GetDataPtr();
    EXPECT_NEAR(ptr[7], 1.80, 0.01);
}

TEST(PressureFieldSerialTest, ReciprocalTest)
{
    PressureFieldSerial pfs = getPFS();
    pfs.Reciprocal();
    double *ptr = pfs.GetDataPtr();
    EXPECT_NEAR(ptr[7], 0.30, 0.01);
}

TEST(PressureFieldSerialTest, InnerProductTest)
{
    PressureFieldSerial pfs = getPFS();
    EXPECT_NEAR(pfs.InnerProduct(pfs), 36.5, 0.01);
}

TEST(PressureFieldSerialTest, GradientTest)
{
    // Make grid with non-boundary cells, i.e. at least 3x3.
    std::array<double, 2> x_min = {0.0, 0.0};
    std::array<double, 2> x_max = {2.0, 2.0};
    std::array<int, 2> n_x = {3, 4};
    Grid2D grid(x_min, x_max, n_x);

    // Assign planar function to grid
    PressureFieldSerial pfs(grid);
    std::function<double(double, double)> func = [](double x, double z) { return x + z; }; // Linear plane, x & z are centroids of grid cell.
    pfs.SetField(func);

    // Prepare output double pointer
    PressureFieldSerial **pfs_out = new PressureFieldSerial *[2];
    pfs_out[0] = new PressureFieldSerial(grid);
    pfs_out[1] = new PressureFieldSerial(grid);

    pfs.Gradient(pfs_out);

    // Retrieve data from pointers
    double *data_ptr[2];
    data_ptr[0] = pfs_out[0]->GetDataPtr();
    data_ptr[1] = pfs_out[1]->GetDataPtr();

    // Assert.
    EXPECT_NEAR(data_ptr[0][0], 1, 0.01);
    EXPECT_NEAR(data_ptr[0][11], 1, 0.01);
    EXPECT_NEAR(data_ptr[1][0], 1, 0.01);
    EXPECT_NEAR(data_ptr[1][11], 1, 0.01);

    for (int i = 0; i < 2; i++)
    {
        delete pfs_out[i];
    }
    delete pfs_out;
}

TEST(PressureFieldSerialTest, ZeroTest)
{
    PressureFieldSerial pfs = getPFS();
    pfs.Zero();
    double *data_pfs = pfs.GetDataPtr();
    EXPECT_NEAR(data_pfs[1], 0.0, 0.001);
}

TEST(PressureFieldSerialTest, OperatorAssignPressureFieldSerialTest)
{
    PressureFieldSerial pfs_1 = getPFS();
    PressureFieldSerial pfs_2 = pfs_1;

    double *data_pfs_1 = pfs_1.GetDataPtr();
    double *data_pfs_2 = pfs_2.GetDataPtr();

    for (int i = 0; i != pfs_1.GetNumberOfGridPoints(); ++i)
    {
        EXPECT_NEAR(data_pfs_1[i], data_pfs_2[i], 0.0001);
    }
}

TEST(PressureFieldSerialTest, OperatorAssignDoubleTest)
{
    PressureFieldSerial pfs_1 = getPFS();
    pfs_1 = 0.1;

    double *data_pfs_1 = pfs_1.GetDataPtr();

    for (int i = 0; i != pfs_1.GetNumberOfGridPoints(); ++i)
    {
        EXPECT_NEAR(data_pfs_1[i], 0.1, 0.0001);
    }
}

TEST(PressureFieldSerialTest, OperatorSubtractPressureFieldSerialTest)
{
    PressureFieldSerial pfs_1 = getPFS();
    PressureFieldSerial pfs_2 = getPFS();
    pfs_1 = 0.2;
    pfs_2 = 0.1;

    pfs_1 -= pfs_2;

    double *data_pfs_1 = pfs_1.GetDataPtr();

    for (int i = 0; i != pfs_1.GetNumberOfGridPoints(); ++i)
    {
        EXPECT_NEAR(data_pfs_1[i], 0.1, 0.0001);
    }
}

TEST(PressureFieldSerialTest, OperatorSubtractDoubleTest)
{
    PressureFieldSerial pfs_1 = getPFS();
    pfs_1.Zero();
    pfs_1 -= 0.1;

    double *data_pfs_1 = pfs_1.GetDataPtr();

    for (int i = 0; i != pfs_1.GetNumberOfGridPoints(); ++i)
    {
        EXPECT_NEAR(data_pfs_1[i], -0.1, 0.0001);
    }
}

TEST(PressureFieldSerialTest, OperatorDivideByPressureFieldSerialTest)
{
    PressureFieldSerial pfs_1 = getPFS();
    PressureFieldSerial pfs_2 = getPFS();
    pfs_1 = 0.2;
    pfs_2 = 0.1;
    pfs_1 /= pfs_2;

    double *data_pfs_1 = pfs_1.GetDataPtr();

    for (int i = 0; i != pfs_1.GetNumberOfGridPoints(); ++i)
    {
        EXPECT_NEAR(data_pfs_1[i], 2.0, 0.0001);
    }
}

TEST(PressureFieldSerialTest, OperatorDivideByDoubleTest)
{
    PressureFieldSerial pfs_1 = getPFS();
    pfs_1 = 0.2;
    pfs_1 /= 2.0;

    double *data_pfs_1 = pfs_1.GetDataPtr();

    for (int i = 0; i != pfs_1.GetNumberOfGridPoints(); ++i)
    {
        EXPECT_NEAR(data_pfs_1[i], 0.1, 0.0001);
    }
}

TEST(PressureFieldSerialTest, OperatorMultiplyByPressureFieldSerialTest)
{
    PressureFieldSerial pfs_1 = getPFS();
    PressureFieldSerial pfs_2 = getPFS();
    pfs_1 = 0.2;
    pfs_2 = 2.0;
    pfs_1 *= pfs_2;

    double *data_pfs_1 = pfs_1.GetDataPtr();

    for (int i = 0; i != pfs_1.GetNumberOfGridPoints(); ++i)
    {
        EXPECT_NEAR(data_pfs_1[i], 0.4, 0.0001);
    }
}

TEST(PressureFieldSerialTest, OperatorMultiplyByDoubleTest)
{
    PressureFieldSerial pfs_1 = getPFS();
    pfs_1 = 0.2;
    pfs_1 *= 2.0;

    double *data_pfs_1 = pfs_1.GetDataPtr();

    for (int i = 0; i != pfs_1.GetNumberOfGridPoints(); ++i)
    {
        EXPECT_NEAR(data_pfs_1[i], 0.4, 0.0001);
    }
}