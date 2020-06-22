#include "grid2D.h"
#include <gtest/gtest.h>

namespace core
{
    std::array<double, 2> x_min = {0.0, 0.0};
    std::array<double, 2> x_max = {2.0, 2.0};
    std::array<int, 2> n_x = {2, 4};

    grid2D grid = grid2D(x_min, x_max, n_x);

    TEST(grid2DTest, getGridDimensionsTest) { EXPECT_EQ(grid.getGridDimensions(), n_x); }

    TEST(grid2DTest, GetCellDimensionsTest)
    {
        std::array<double, 2> a = grid.getCellDimensions();

        std::array<double, 2> b;
        b[0] = 2.0 / 2.0;
        b[1] = 2.0 / 4.0;

        for(int i = 0; i < 2; i++)
        {
            EXPECT_NEAR(a[i], b[i], 0.01);
        }
    }

    TEST(grid2DTest, GetGridStartTest) { EXPECT_EQ(grid.getGridStart(), x_min); }

    TEST(grid2DTest, GetGridEndTest) { EXPECT_EQ(grid.getGridEnd(), x_max); }

    TEST(grid2DTest, GetNumberOfGridPointsTest) { EXPECT_EQ(grid.getNumberOfGridPoints(), 8); }

    TEST(grid2DTest, GetCellVolumeTest) { EXPECT_NEAR(grid.getCellVolume(), 0.5, 0.01); }

    TEST(grid2DTest, getDomainAreaTest) { EXPECT_NEAR(grid.getDomainArea(), (x_max[0] - x_min[0]) * (x_max[1] - x_min[1]), 0.01); }

    TEST(grid2DTest, booleanOperatorEqualTest)
    {
        grid2D grid1 = grid2D(x_min, x_max, n_x);
        grid2D grid2 = grid2D(x_min, x_max, n_x);

        bool gridsAreEqual = grid1 == grid2;

        EXPECT_TRUE(gridsAreEqual);
    }

    TEST(grid2DTest, booleanOperatorUnequalXMinTest)
    {
        grid2D grid1 = grid2D(x_min, x_max, n_x);

        std::array<double, 2> x_min2 = {0.1, 0.3};
        grid2D grid2 = grid2D(x_min2, x_max, n_x);

        bool gridsAreEqual = grid1 == grid2;

        EXPECT_FALSE(gridsAreEqual);
    }

    TEST(grid2DTest, booleanOperatorUnequalXMaxTest)
    {
        grid2D grid1 = grid2D(x_min, x_max, n_x);

        std::array<double, 2> x_max2 = {2.2, 1.8};
        grid2D grid2 = grid2D(x_min, x_max2, n_x);

        bool gridsAreEqual = grid1 == grid2;

        EXPECT_FALSE(gridsAreEqual);
    }

    TEST(grid2DTest, booleanOperatorUnequalDxTest)
    {
        grid2D grid1 = grid2D(x_min, x_max, n_x);

        std::array<int, 2> n_x2 = {3, 6};
        grid2D grid2 = grid2D(x_min, x_max, n_x2);

        bool gridsAreEqual = grid1 == grid2;

        EXPECT_FALSE(gridsAreEqual);
    }
}   // namespace core
