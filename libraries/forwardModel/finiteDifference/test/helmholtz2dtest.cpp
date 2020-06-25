#include <../include/helmholtz2D.h>
#include <gtest/gtest.h>

namespace fwi
{
    namespace forwardModels
    {
        static const double pi = std::atan(1.0) * 4.0;

        TEST(helmholtz2dTest, CreateABCMatrixTest)
        {
            std::array<double, 2> x_min = {0.0, 0.0};
            std::array<double, 2> x_max = {2.0, 2.0};
            std::array<int, 2> n_x = {2, 4};

            std::array<double, 2> SrcMin = {0.0, -2.0};
            std::array<double, 2> SrcMax = {2.0, -2.0};
            int nSrc = 2;

            core::grid2D grid = core::grid2D(x_min, x_max, n_x);

            double freq = 20.0;
            double c_0 = 2000.0;
            core::sources src(SrcMin, SrcMax, nSrc);

            core::dataGrid2D chiEst(grid);
            chiEst.random();

            finiteDifferenceForwardModelInput fmInput;
            PMLWidthFactor pmlWidthfactor;
            pmlWidthfactor.x = 0.0;
            pmlWidthfactor.z = 0.0;
            SourceParameter sourceParameter;
            sourceParameter.r = 4, sourceParameter.beta = 6.31;
            fmInput.pmlWidthFactor = pmlWidthfactor;
            fmInput.sourceParameter = sourceParameter;

            Helmholtz2D helmholtzFreq(grid, freq, src, c_0, chiEst, fmInput);
            std::array<double, 2> dx;
            dx[0] = (x_max[0] - x_min[0]) / static_cast<double>(n_x[0] - 1);
            dx[1] = (x_max[1] - x_min[1]) / static_cast<double>(n_x[1] - 1);

            double omega = freq * 2.0 * pi;
            std::vector<Eigen::Triplet<std::complex<double>>> triplets;
            triplets.reserve(5 * n_x[0] * n_x[1]);   // Naive upper bound for nnz's

            helmholtzFreq.CreateABCMatrix(omega, dx, triplets, n_x);

            EXPECT_GT(triplets.size(), 0.0);
        }

        TEST(helmholtz2dTest, CreateABCSecondOrderMatrixTest)
        {
            std::array<double, 2> x_min = {0.0, 0.0};
            std::array<double, 2> x_max = {2.0, 2.0};
            std::array<int, 2> n_x = {2, 4};

            std::array<double, 2> SrcMin = {0.0, -2.0};
            std::array<double, 2> SrcMax = {2.0, -2.0};
            int nSrc = 2;

            core::grid2D grid = core::grid2D(x_min, x_max, n_x);

            double freq = 20.0;
            double c_0 = 2000.0;
            core::sources src(SrcMin, SrcMax, nSrc);

            core::dataGrid2D chiEst(grid);
            chiEst.random();

            finiteDifferenceForwardModelInput fmInput;
            PMLWidthFactor pmlWidthfactor;
            pmlWidthfactor.x = 0.0;
            pmlWidthfactor.z = 0.0;
            SourceParameter sourceParameter;
            sourceParameter.r = 4, sourceParameter.beta = 6.31;
            fmInput.pmlWidthFactor = pmlWidthfactor;
            fmInput.sourceParameter = sourceParameter;

            Helmholtz2D helmholtzFreq(grid, freq, src, c_0, chiEst, fmInput);
            std::array<double, 2> dx;
            dx[0] = (x_max[0] - x_min[0]) / static_cast<double>(n_x[0] - 1);
            dx[1] = (x_max[1] - x_min[1]) / static_cast<double>(n_x[1] - 1);

            double omega = freq * 2.0 * pi;
            std::vector<Eigen::Triplet<std::complex<double>>> triplets;
            triplets.reserve(5 * n_x[0] * n_x[1]);   // Naive upper bound for nnz's

            helmholtzFreq.CreateABCSecondOrderMatrix(omega, dx, triplets, n_x);

            EXPECT_GT(triplets.size(), 0.0);
        }
    }   // namespace forwardModels
}   // namespace fwi
