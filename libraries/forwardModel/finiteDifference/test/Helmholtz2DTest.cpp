#include "Helmholtz2D.h"
#include <gtest/gtest.h>

namespace fwi
{
    namespace forwardModels
    {
        class Helmholtz2DTest : public ::testing::Test
        {
            protected:

                const double pi = std::atan(1.0) * 4.0;

                // grid
                std::array<double, 2> xMin = {0.0, 0.0};
                std::array<double, 2> xMax = {2.0, 2.0};

                // source
                std::array<double, 2> sourceMin = {0.0, 0.0};
                std::array<double, 2> sourceMax = {2.0, 0.0};

                // pmlWidthFactor
                double pmlWidthFactorX = 0.0;
                double pmlWidthFactorZ = 0.0;

                // sourceParameters
                int sourceParameterR = 0;
                double sourceParameterBeta = 6.31;

                Eigen::MatrixXcd createDenseMatrix(const std::array<int, 2> &nx, const BoundaryConditionType &matrixType, double chiValue, double c0, double freq)
                {
                    // create 2d grid
                    core::grid2D grid = core::grid2D(xMin, xMax, nx);

                    // create sources
                    int count = 2;
                    core::Sources source(sourceMin, sourceMax, count);

                    // chiEst
                    core::dataGrid2D chiEst(grid);
                    chiEst = chiValue;

                    // fmInput
                    finiteDifferenceForwardModelInput fmInput;

                    PMLWidthFactor pmlWidthfactor;
                    pmlWidthfactor.x = pmlWidthFactorX;
                    pmlWidthfactor.z = pmlWidthFactorZ;
                    fmInput.pmlWidthFactor = pmlWidthfactor;

                    SourceParameter sourceParameter;
                    sourceParameter.r = sourceParameterR;
                    sourceParameter.beta = sourceParameterBeta;
                    fmInput.sourceParameter = sourceParameter;

                    fmInput.boundaryConditionType = matrixType;

                    Helmholtz2D helmholtzFreq(grid, freq, source, c0, chiEst, fmInput);

                    Eigen::MatrixXcd denseMatrix = helmholtzFreq.getSparseMatrixA();
                    return denseMatrix;
                }

        };

        TEST_F(Helmholtz2DTest, twoByTwoGrid_validInput)
        {
            //Arrange

            double c0 = 1.0;
            double freq = 1.0;

            // this determines the size of the grid
            std::array<int, 2> nx = {2, 2};
            int matrixSize = nx[0] * nx[1];

            Eigen::MatrixXcd expectedMatrix(matrixSize, matrixSize);

            // the elements in the matrix have been filled in while showing the contribution from individual finite difference nodes
            expectedMatrix(0,0) = -1.0 - 0.5;
            expectedMatrix(1,0) = 0.25 + 0.25;
            expectedMatrix(2,0) = 0.25 + 0.50;
            expectedMatrix(3,0) = 0;

            expectedMatrix(0,1) = 0.25 + 0.25;
            expectedMatrix(1,1) = -1.0 - 0.5;
            expectedMatrix(2,1) = 0;
            expectedMatrix(3,1) = 0.25 + 0.50;

            expectedMatrix(0,2) = 0.25 + 0.50;
            expectedMatrix(1,2) = 0;
            expectedMatrix(2,2) = -1.0 - 0.5;
            expectedMatrix(3,2) = 0.25 + 0.25;

            expectedMatrix(0,3) = 0;
            expectedMatrix(1,3) = 0.25 + 0.50;
            expectedMatrix(2,3) = 0.25 + 0.25;
            expectedMatrix(3,3) = -1.0 - 0.5;


            // Act

            BoundaryConditionType boundaryConditionType = BoundaryConditionType::SecondOrderABC;
            double chiValue = 1.0;
            Eigen::MatrixXcd secondOrderABCMatrix = createDenseMatrix(nx, boundaryConditionType, chiValue, c0, freq);

            // Assert
            // Eigen::matrix can be parsed in a columnMajor way by using only one index
            for(int it = 0; it < expectedMatrix.size(); ++it)
            {
                EXPECT_DOUBLE_EQ(secondOrderABCMatrix(it).real(), expectedMatrix(it).real());
                EXPECT_DOUBLE_EQ(secondOrderABCMatrix(it).imag(), expectedMatrix(it).imag());
            }
        }

        TEST_F(Helmholtz2DTest, threeByThreeGrid_validInput)
        {
            //Arrange

            double c0 = 1.0;
            double freq = 1.0;

            double testTolerance = 1e-3;
            std::array<int, 2> nx = {3, 3};
            int matrixSize = nx[0] * nx[1];

            Eigen::MatrixXcd expectedMatrix(matrixSize, matrixSize);

            // the elements in the matrix haave been filled in while showing the contribution from individual finite difference nodes

            // top boundary of the grid
            expectedMatrix(0,0) = std::complex<double> (35.478,0) + std::complex<double> (-2,18.849);
            expectedMatrix(1,0) = std::complex<double> (1,0) + std::complex<double> (0,0.159);
            expectedMatrix(2,0) = 0.0;
            expectedMatrix(3,0) = std::complex<double> (1,0) + std::complex<double> (0,0.159);;
            expectedMatrix(4,0) = 0.0;
            expectedMatrix(5,0) = 0.0;
            expectedMatrix(6,0) = 0.0;
            expectedMatrix(7,0) = 0.0;
            expectedMatrix(8,0) = 0.0;

            // Act

            BoundaryConditionType boundaryConditionType = BoundaryConditionType::SecondOrderABC;
            double chiValue = 0.0;
            Eigen::MatrixXcd secondOrderABCMatrix = createDenseMatrix(nx, boundaryConditionType, chiValue, c0, freq);

            // Assert

            for(int it = 0; it < matrixSize; ++it)
            {
                ASSERT_NEAR(expectedMatrix(it,0).real(), secondOrderABCMatrix(it,0).real(), testTolerance);
                ASSERT_NEAR(expectedMatrix(it,0).imag(), secondOrderABCMatrix(it,0).imag(), testTolerance);
            }
        }

    }   // namespace forwardModels
}   // namespace fwi
