#include "dataGrid2D.h"
#include <gtest/gtest.h>
#include <iostream>

namespace fwi
{
    namespace core
    {
        grid2D getGrid()
        {
            std::array<double, 2> xMin = {0.0, 0.0};
            std::array<double, 2> xMax = {2.0, 2.0};
            std::array<int, 2> nX = {2, 4};

            grid2D grid(xMin, xMax, nX);
            return grid;
        }

        TEST(dataGrid2DComplexTest, addValueAtIndexTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue = 13;
            int index = 3;
            dataGrid2D<std::complex<double>>cdg(grid);

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

        TEST(dataGrid2DComplexTest, copyConstructorTest)
        {
            // Given
            std::complex<double> testValue(3.0, 4.6);
            grid2D grid = getGrid();
            dataGrid2D<std::complex<double>>cdg1(grid);
            cdg1 = testValue;
\
            // When
            dataGrid2D<std::complex<double>>cdg2(cdg1);

            // Then
            const int nrOfGridPoints = cdg2.getNumberOfGridPoints();
            const std::vector<std::complex<double>> &data = cdg2.getData();
            for(int i = 0; i < nrOfGridPoints; i++)
            {
                ASSERT_DOUBLE_EQ(data[i].real(), real(testValue));
                ASSERT_DOUBLE_EQ(data[i].imag(), imag(testValue));
            }
        }

        TEST(dataGrid2DComplexTest, zeroTest)
        {
            // Given
            grid2D grid = getGrid();

            std::complex<double> testValue(1.0, 1.0);
            dataGrid2D<std::complex<double>>cdg(grid);
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

        TEST(dataGrid2DComplexTest, squareTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue(1.0, 2.0);
            dataGrid2D<std::complex<double>>cdg(grid);
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

        TEST(dataGrid2DComplexTest, sqrtTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue(4.0, 9.0);
            dataGrid2D<std::complex<double>>cdg(grid);
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

        TEST(dataGrid2DDoubleTest, sqrtTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            double value = 36.0;
            dataGrid2D<double> dg(grid);
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

        TEST(dataGrid2DComplexTest, reciprocalTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue(1.0, 2.0);
            dataGrid2D<std::complex<double>>cdg(grid);
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

        TEST(dataGrid2DDoubleTest, reciprocalTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            double value = 2.0;
            dataGrid2D<double> dg(grid);
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

        TEST(dataGrid2DComplexTest, conjugateTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue(1.0, 2.0);
            dataGrid2D<std::complex<double>>cdg(grid);
            cdg = testValue;

            // When
            cdg.conjugate();

            // Then
            std::complex<double> testSolution = std::conj(testValue);
            const std::vector<std::complex<double>> &data = cdg.getData();
            for(int i = 0; i < nrOfGridPoints; i++)
            {
                ASSERT_DOUBLE_EQ(data[i].real(), real(testSolution));
                ASSERT_DOUBLE_EQ(data[i].imag(), imag(testSolution));
            }
        }

        TEST(dataGrid2DDoubleTest, conjugateTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            double value = 2.0;
            dataGrid2D<double> dg(grid);
            dg = value;

            // When
            dg.conjugate();

            // Then
            // Value should not have changed, since conjugate of a non-complex number
            // is just the same number.
            double solution = value;

            const std::vector<double> &data = dg.getData();
            for(int i = 0; i < nrOfGridPoints; i++)
            {
                ASSERT_DOUBLE_EQ(data[i], solution);
            }
        }

        TEST(dataGrid2DComplexTest, reciprocalExceptionTest)
        {
            // Given
            grid2D grid = getGrid();
            dataGrid2D<std::complex<double>>cdg(grid);
            // Note: Initialized with zero

            // When
            // Nothing here

            // Then
            EXPECT_THROW(cdg.reciprocal(), std::overflow_error);
            // Note: 1 / 0.0 must throw
        }

        TEST(dataGrid2DDoubleTest, normTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            double value = 1.0;
            dataGrid2D<double> dg(grid);
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

        TEST(dataGrid2DDoubleTest, relNormTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            double value = 2.0;
            dataGrid2D<double> dg(grid);
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


        TEST(dataGrid2DComplexTest, summationTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue = 2.0;
            dataGrid2D<std::complex<double>>cdg(grid);
            cdg = testValue;

            // Alternative calculation method
            std::complex<double> summedValue = testValue * (double) nrOfGridPoints;

            // When
            std::complex<double> summationResult = cdg.summation();

            // Then
            ASSERT_DOUBLE_EQ(summationResult.real(), real(summedValue));
            ASSERT_DOUBLE_EQ(summationResult.imag(), imag(summedValue));
        }

        TEST(dataGrid2DComplexTest, innerProductTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue = {2.0, 1.0};
            dataGrid2D<std::complex<double>>cdg(grid);
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

        TEST(dataGrid2DDoubleTest, innerProductTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            double value = 2.0;
            dataGrid2D<double> dg(grid);
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


        TEST(dataGrid2DComplexTest, dotProductTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue = {2.0, 28.0};
            dataGrid2D<std::complex<double>>cdg(grid);
            cdg = testValue;

            // Alternative calculation method
            std::complex<double> solution = testValue * testValue * (double) nrOfGridPoints;

            // When
            std::complex<double> calculatedDotProduct = cdg.dotProduct(cdg);

            // Then
            ASSERT_DOUBLE_EQ(solution.real(), calculatedDotProduct.real());
            ASSERT_DOUBLE_EQ(solution.imag(), calculatedDotProduct.imag());
        }

        TEST(dataGrid2DDoubleTest, dotProductTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            double testValue = 2.0;
            dataGrid2D<double> dg(grid);
            dg = testValue;

            double solution = testValue * testValue * (double) nrOfGridPoints;

            // When
            double calculatedDotProduct = dg.dotProduct(dg);

            // Then
            ASSERT_DOUBLE_EQ(solution, calculatedDotProduct);
        }

        TEST(dataGrid2DDoubleTest, gradientTest)
        {
            // Given
            // Make grid with non-boundary cells, i.e. at least 3x3.
            const std::array<double, 2> xMin = {0.0, 0.0};
            const std::array<double, 2> xMax = {2.0, 2.0};
            const std::array<int, 2> nX = {3, 4};
            grid2D grid(xMin, xMax, nX);
            const int nrOfGridPoints = grid.getNumberOfGridPoints();
            const std::array<double, 2> &dx = grid.getCellDimensions();

            dataGrid2D<double> dg(grid);
            std::vector<dataGrid2D<double>> dgOut(2, dataGrid2D<double>(grid));

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
        TEST(dataGrid2DComplexTest, operatorAssigndataGrid2DExceptionTest)
        {
            // Given
            grid2D grid = getGrid();
            dataGrid2D<std::complex<double>> cdg(grid);

            // When
            // Nothing here, see below

            // Then
            EXPECT_THROW(cdg = cdg, std::logic_error);
        }

        TEST(dataGrid2DComplexTest, operatorAssigndataGrid2DTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue(0.0, 0.0);
            dataGrid2D<std::complex<double>>cdg1(grid);
            cdg1 = testValue;

            dataGrid2D<std::complex<double>> cdg2(grid);
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

        TEST(dataGrid2DComplexTest, operatorAssignComplexDoubleVectorTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();
            dataGrid2D<std::complex<double>> cdg(grid);

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

        TEST(dataGrid2DComplexTest, operatorAssignDoubleVectorTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();
            dataGrid2D<std::complex<double>> cdg(grid);

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

        TEST(dataGrid2DComplexTest, operatorAssignComplexDoubleVectorTest2)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue(1.0, 2.0);
            dataGrid2D<std::complex<double>> cdg(grid);
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

        TEST(dataGrid2DDoubleTest, operatorAssignDoubleVectorTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();
            dataGrid2D<double> cdg(grid);

            double testValue = 1.0;
            std::vector<double> dataVector = std::vector<double>(nrOfGridPoints, testValue);

            // When
            cdg = dataVector;

            // Then
            const std::vector<double> &data = cdg.getData();
            for(int i = 0; i < nrOfGridPoints; i++)
            {
                ASSERT_DOUBLE_EQ(data[i], testValue);
            }
        }

        TEST(dataGrid2DDoubleTest, operatorAssignDoubleTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();
            dataGrid2D<double> cdg(grid);

            double testValue = 1.0;

            // When
            cdg = testValue;

            // Then
            const std::vector<double> &data = cdg.getData();
            for(int i = 0; i < nrOfGridPoints; i++)
            {
                ASSERT_DOUBLE_EQ(data[i], testValue);
            }
        }

        TEST(dataGrid2DDoubleTest, operatorAssignDoubleVectorTest2)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            double testValue = 1.0;
            dataGrid2D<double> cdg(grid);
            cdg = testValue;

            std::vector<double> dataIncreasing = std::vector<double>(nrOfGridPoints, 0.0);
            for(int i = 0; i < nrOfGridPoints; i++)
            {
                dataIncreasing[i] = i;
            }

            // When
            cdg = dataIncreasing;

            // Then
            const std::vector<double> &data = cdg.getData();
            for(int i = 0; i < nrOfGridPoints; i++)
            {
                ASSERT_DOUBLE_EQ(data[i], double(i));
            }
        }

        TEST(dataGrid2DComplexTest, operatorAssignComplexDoubleTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();
            dataGrid2D<std::complex<double>> cdg(grid);

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

        TEST(dataGrid2DComplexTest, operatorAssignDoubleTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();
            dataGrid2D<std::complex<double>> cdg(grid);

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

        TEST(dataGrid2DComplexTest, operatorAdddataGrid2DTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue1(1.0, 2.0);
            dataGrid2D<std::complex<double>> cdg1(grid);
            cdg1 = testValue1;

            std::complex<double> testValue2(2.0, 3.0);
            dataGrid2D<std::complex<double>> cdg2(grid);
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

        TEST(dataGrid2DComplexTest, operatorAdddataGrid2DTest2)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue(1.0, 2.0);
            dataGrid2D<std::complex<double>> cdg1(grid);
            cdg1 = testValue;

            std::complex<double> count(0.0, 0.0);
            std::complex<double> increment(1.0, 1.0);
            std::vector<std::complex<double>> dataIncreasing = std::vector<std::complex<double>>(nrOfGridPoints, 0.0);
            for(int i = 0; i < nrOfGridPoints; i++)
            {
                dataIncreasing[i] = count;
                count += increment;
            }

            dataGrid2D<std::complex<double>> cdg2(grid);
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

        TEST(dataGrid2DComplexTest, operatorAddComplexDoubleVectorTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue1(2.0, 3.0);
            dataGrid2D<std::complex<double>> cdg(grid);
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

        TEST(dataGrid2DComplexTest, operatorAddComplexDoubleVectorTest2)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue(1.0, 2.0);
            dataGrid2D<std::complex<double>> cdg(grid);
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

        TEST(dataGrid2DComplexTest, operatorAddDoubleVectorTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            double testValue = 1.0;
            dataGrid2D<std::complex<double>> cdg(grid);

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

        TEST(dataGrid2DComplexTest, operatorAddDoubleVectorTest2)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue(2.1, 3.1);
            dataGrid2D<std::complex<double>> cdg(grid);
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

        TEST(dataGrid2DComplexTest, operatorAddComplexDoubleTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue1(1.0, 2.0);
            dataGrid2D<std::complex<double>> cdg(grid);
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

        TEST(dataGrid2DComplexTest, operatorAddDoubleTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue1(2.1, 3.1);
            dataGrid2D<std::complex<double>> cdg(grid);
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

        TEST(dataGrid2DComplexTest, operatorSubtractdataGrid2DTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue1 = 9.2;
            dataGrid2D<std::complex<double>> cdg1(grid);
            cdg1 = testValue1;

            std::complex<double> testValue2 = 4.8;
            dataGrid2D<std::complex<double>> cdg2(grid);
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

        TEST(dataGrid2DComplexTest, operatorSubtractdataGrid2DTest2)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue(21.0, 22.0);
            dataGrid2D<std::complex<double>> cdg1(grid);
            cdg1 = testValue;

            std::complex<double> count(0.0, 0.0);
            std::complex<double> increment(1.0, 1.0);
            std::vector<std::complex<double>> dataIncreasing = std::vector<std::complex<double>>(nrOfGridPoints, 0.0);
            for(int i = 0; i < nrOfGridPoints; i++)
            {
                dataIncreasing[i] = count;
                count += increment;
            }

            dataGrid2D<std::complex<double>> cdg2(grid);
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

        TEST(dataGrid2DComplexTest, operatorSubtractDataGrid2DTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue1(1.0, 2.0);
            dataGrid2D<std::complex<double>> cdg(grid);
            cdg = testValue1;

            double testValue2 = 2.0;
            dataGrid2D<double> dg(grid);
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

        TEST(dataGrid2DComplexTest, operatorSubtractDataGrid2DTest2)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue(1.0, 2.0);
            dataGrid2D<std::complex<double>> cdg(grid);
            cdg = testValue;

            double count = 0;
            std::vector<double> dataIncreasing = std::vector<double>(nrOfGridPoints, 0.0);
            for(int i = 0; i < nrOfGridPoints; i++)
            {
                dataIncreasing[i] = count;
                count++;
            }

            dataGrid2D<double> dg(grid);
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

        TEST(dataGrid2DComplexTest, operatorSubtractComplexDoubleVectorTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue1(2.0, 3.0);
            dataGrid2D<std::complex<double>> cdg(grid);
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

        TEST(dataGrid2DComplexTest, operatorSubtractComplexDoubleVectorTest2)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue(21.0, 22.0);
            dataGrid2D<std::complex<double>> cdg(grid);
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

        TEST(dataGrid2DComplexTest, operatorSubtractDoubleVectorTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue1(2.1, 3.1);
            dataGrid2D<std::complex<double>> cdg(grid);
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

        TEST(dataGrid2DComplexTest, operatorSubtractDoubleVectorTest2)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue(2.1, 3.1);
            dataGrid2D<std::complex<double>> cdg(grid);
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

        TEST(dataGrid2DComplexTest, operatorSubtractComplexDoubleTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue1(31.0, 32.0);
            dataGrid2D<std::complex<double>> cdg(grid);
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

        TEST(dataGrid2DComplexTest, operatorSubtractAssignComplex)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue1(2.1, 3.1);
            dataGrid2D<std::complex<double>> cdg(grid);
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

        TEST(dataGrid2DComplexTest, operatorMultiplyAssignBydataGrid2DComplex)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue1(2.1, 3.1);
            dataGrid2D<std::complex<double>> cdg1(grid);
            cdg1 = testValue1;

            std::complex<double> testValue2(3.1, 4.1);
            dataGrid2D<std::complex<double>> cdg2(grid);
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

        TEST(dataGrid2DComplexTest, operatorMultiplyAssignBydataGrid2DComplex2)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue(1.0, 2.0);
            dataGrid2D<std::complex<double>> cdg1(grid);
            cdg1 = testValue;

            std::complex<double> count(0.0, 0.0);
            std::complex<double> increment(1.0, 1.0);
            std::vector<std::complex<double>> dataIncreasing = std::vector<std::complex<double>>(nrOfGridPoints, 0.0);
            for(int i = 0; i < nrOfGridPoints; i++)
            {
                dataIncreasing[i] = count;
                count += increment;
            }

            dataGrid2D<std::complex<double>> cdg2(grid);
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

        TEST(dataGrid2DComplexTest, operatorMultiplyByDataGrid2DTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue1(1.0, 2.0);
            dataGrid2D<std::complex<double>> cdg(grid);
            cdg = testValue1;

            double testValue2 = 2.0;
            dataGrid2D<double> dg(grid);
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

        TEST(dataGrid2DComplexTest, operatorMultiplyByDataGrid2DTest2)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue(1.0, 2.0);
            dataGrid2D<std::complex<double>> cdg(grid);
            cdg = testValue;

            double count = 0;
            std::vector<double> dataIncreasing = std::vector<double>(nrOfGridPoints, 0.0);
            for(int i = 0; i < nrOfGridPoints; i++)
            {
                dataIncreasing[i] = count;
                count++;
            }

            dataGrid2D<double> dg(grid);
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

        TEST(dataGrid2DComplexTest, operatorMultiplyByComplexDoubleVectorTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue1(2.0, 3.0);
            dataGrid2D<std::complex<double>> cdg(grid);
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

        TEST(dataGrid2DComplexTest, operatorMultiplyByComplexDoubleVectorTest2)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue(21.0, 22.0);
            dataGrid2D<std::complex<double>> cdg(grid);
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

        TEST(dataGrid2DComplexTest, operatorMultiplyByDoubleVectorTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue1(2.1, 3.1);
            dataGrid2D<std::complex<double>> cdg(grid);
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

        TEST(dataGrid2DComplexTest, operatorMultiplyByDoubleVectorTest2)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue(2.1, 3.1);
            dataGrid2D<std::complex<double>> cdg(grid);
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

        TEST(dataGrid2DComplexTest, operatorMultiplyByComplexDoubleTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue1(2.2, 3.3);
            dataGrid2D<std::complex<double>> cdg(grid);
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

        TEST(dataGrid2DComplexTest, operatorMultiplyByDoubleTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue1(2.1, 3.1);
            dataGrid2D<std::complex<double>> cdg(grid);
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

        TEST(dataGrid2DTest, operatorMultiplyDyDataGridDoubleTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> gridValue1(2.1, 3.1);
            dataGrid2D<std::complex<double>> cdg(grid);
            cdg = gridValue1;

            // When
            double gridValue2 = 2.0;
            dataGrid2D<double> dg(grid);
            dg = gridValue2;

            // Then
            dataGrid2D<std::complex<double>> dgmult = cdg * dg;
            const std::vector<std::complex<double>> &data = dgmult.getData();
            std::complex<double> testSolution = gridValue1 * gridValue2;
            for(int i = 0; i < nrOfGridPoints; i++)
            {
                ASSERT_DOUBLE_EQ(data[i].real(), real(testSolution));
                ASSERT_DOUBLE_EQ(data[i].imag(), imag(testSolution));
            }
        }

        TEST(dataGrid2DComplexTest, operatorDivideByPressureFieldComplexSerialExceptionTest)
        {
            // Given
            grid2D grid = getGrid();

            std::complex<double> testValue1(3.7, 6.2);
            dataGrid2D<std::complex<double>> cdg1(grid);
            cdg1 = testValue1;

            dataGrid2D<std::complex<double>> cdg2(grid);
            // Note: Initialized with zero

            // When
            // Nothing here, see below

            // Then
            EXPECT_THROW(cdg1 /= cdg2, std::overflow_error);
            // Note: 1 / 0.0 must throw
        }

        TEST(dataGrid2DComplexTest, operatorDivideBydataGrid2DComplexTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue1(-3.7, -2.7);
            dataGrid2D<std::complex<double>> cdg1(grid);
            cdg1 = testValue1;

            std::complex<double> testValue2(7.2, 3.9);
            dataGrid2D<std::complex<double>> cdg2(grid);
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

        TEST(dataGrid2DComplexTest, operatorDivideByDataGrid2DDoubleExceptionTest)
        {
            // Given
            grid2D grid = getGrid();

            std::complex<double> testValue1(1.0, 1.0);
            dataGrid2D<std::complex<double>> cdg(grid);
            cdg = testValue1;

            dataGrid2D<double> dg(grid);
            // Note: Initialized with zero

            // When
            // Nothing here, see below

            // Then
            EXPECT_THROW(cdg /= dg, std::overflow_error);
            // Note: 1 / 0.0 must throw
        }

        TEST(dataGrid2DComplexTest, operatorDivideByDataGrid2DDoubleTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue1(3.7, 6.2);
            dataGrid2D<std::complex<double>> cdg(grid);
            cdg = testValue1;

            double testValue2 = 9.3;
            dataGrid2D<double> dg(grid);
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

        TEST(dataGrid2DComplexTest, operatorDivideByComplexDoubleVectorExceptionTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue1(1.0, 1.0);
            dataGrid2D<std::complex<double>> cdg(grid);
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

        TEST(dataGrid2DComplexTest, operatorDivideByComplexDoubleVectorTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue1(3.8, 1.1);
            dataGrid2D<std::complex<double>> cdg(grid);
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

        TEST(dataGrid2DComplexTest, operatorDivideByComplexDoubleVectorTest2)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue(6.7, -5.3);
            dataGrid2D<std::complex<double>> cdg(grid);
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

        TEST(dataGrid2DComplexTest, operatorDivideByDoubleVectorExceptionTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue1(1.0, 1.0);
            dataGrid2D<std::complex<double>> cdg(grid);
            cdg = testValue1;

            std::vector<double> dataVector = std::vector<double>(nrOfGridPoints, 0.0);
            // Note: Initialized with zero

            // When
            // Nothing here, see below

            // Then
            EXPECT_THROW(cdg /= dataVector, std::overflow_error);
            // Note: 1 / 0.0 must throw
        }

        TEST(dataGrid2DComplexTest, operatorDivideByDoubleVectorTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue1(3.8, 1.1);
            dataGrid2D<std::complex<double>> cdg(grid);
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

        TEST(dataGrid2DComplexTest, operatorDivideByDoubleVectorTest2)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue(6.7, -5.3);
            dataGrid2D<std::complex<double>> cdg(grid);
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

        TEST(dataGrid2DComplexTest, operatorDivideByComplexDoubleExceptionTest)
        {
            // Given
            grid2D grid = getGrid();

            std::complex<double> testValue(1.0, 1.0);
            dataGrid2D<std::complex<double>> cdg(grid);
            cdg = testValue;

            std::complex<double> zero(0.0, 0.0);
            // When
            // Nothing here, see below

            // Then
            EXPECT_THROW(cdg /= zero, std::overflow_error);
            // Note: 1 / 0.0 must throw
        }

        TEST(dataGrid2DComplexTest, operatorDivideByComplexDoubleTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue1(4.6, -0.7);
            dataGrid2D<std::complex<double>> cdg(grid);
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

        TEST(dataGrid2DComplexTest, operatorDivideByDoubleExceptionTest)
        {
            // Given
            grid2D grid = getGrid();

            std::complex<double> testValue1(1.0, 1.0);
            dataGrid2D<std::complex<double>> cdg(grid);
            cdg = testValue1;

            // When
            // Nothing here, see below

            // Then
            EXPECT_THROW(cdg /= 0.0, std::overflow_error);
            // Note: 1 / 0.0 must throw
        }

        TEST(dataGrid2DComplexTest, operatorDivideByDoubleTest)
        {
            // Given
            grid2D grid = getGrid();
            const int nrOfGridPoints = grid.getNumberOfGridPoints();

            std::complex<double> testValue1(15.0, -1.9);
            dataGrid2D<std::complex<double>> cdg(grid);
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

    }   // namespace core
}   // namespace fwi
