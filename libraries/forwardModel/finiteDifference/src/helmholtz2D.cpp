#include "helmholtz2D.h"
#include "log.h"

namespace fwi {
    static const double pi = std::atan(1.0) * 4.0;
}
Helmholtz2D::Helmholtz2D(const Grid2D &grid, const double freq, const Sources &src, const double c0, const PressureFieldSerial &chi, const FiniteDifferenceForwardModelInput &fmInput)
    : _A(), _b(), _oldgrid(grid), _newgrid(), _PMLwidth(), _freq(freq), _c0(c0), _waveVelocity(), _solver(), _srcInput(fmInput.sourceParameter)
{
    double waveLength = _c0 / freq;

    //calculating the width of the perfectly matching layer. (rounded up)
    std::array<double, 2> dx;
    std::array<int, 2> oldnx = grid.GetGridDimensions();
    std::array<double, 2> xMin = grid.GetGridStart();
    std::array<double, 2> xMax = grid.GetGridEnd();

    // Adjust the dx, because they are different in Finite Difference context
    dx[0] = (xMax[0] - xMin[0]) / static_cast<double>(oldnx[0] - 1);
    dx[1] = (xMax[1] - xMin[1]) / static_cast<double>(oldnx[1] - 1);

    _PMLwidth[0] = static_cast< int >( std::ceil(fmInput.pmlWidthFactor.x * waveLength / dx[0]) );
    _PMLwidth[1] = static_cast< int >( std::ceil(fmInput.pmlWidthFactor.z * waveLength / dx[1]) );

    // r == 0 uses the Point source implemtation (see BuildVector)
    double r = static_cast<double>(fmInput.sourceParameter.r);

    //sources can be outside imaging domain. Area that we solve for needs to include all Sources
    double extraWidthLeft = 0.0;
    double extraWidthRight = 0.0;
    double extraHeightBottom = 0.0;
    double extraHeightTop = 0.0;

    const std::array<double, 2> srcXmax = *max_element(src.xSrc.begin(), src.xSrc.end(), [](const std::array<double, 2> &lhs, const std::array<double, 2> &rhs) { return lhs[0] < rhs[0]; });
    const std::array<double, 2> srcXmin = *min_element(src.xSrc.begin(), src.xSrc.end(), [](const std::array<double, 2> &lhs, const std::array<double, 2> &rhs) { return lhs[0] < rhs[0]; });

    const std::array<double, 2> srcZmax = *max_element(src.xSrc.begin(), src.xSrc.end(), [](const std::array<double, 2> &lhs, const std::array<double, 2> &rhs) { return lhs[1] < rhs[1]; });
    const std::array<double, 2> srcZmin = *min_element(src.xSrc.begin(), src.xSrc.end(), [](const std::array<double, 2> &lhs, const std::array<double, 2> &rhs) { return lhs[1] < rhs[1]; });

    if (xMin[0] - srcXmin[0] + dx[0] * r > extraWidthLeft)
    {
        extraWidthLeft = xMin[0] - srcXmin[0] + dx[0] * r;
    }
    if (srcXmax[0] - xMax[0] + dx[0] * r > extraWidthRight)
    {
        extraWidthRight = srcXmax[0] - xMax[0] + dx[0] * r;
    }

    if (xMin[1] - srcZmax[1] + dx[1] * r > extraHeightTop)
    {
        extraHeightTop = xMin[1] - srcZmax[1] + dx[1] * r;
    }
    if (srcZmin[1] - xMax[1] + dx[1] * r > extraHeightBottom)
    {
        extraHeightBottom = srcZmin[1] - xMax[1] + dx[1] * r;
    }

    int extraGridPointsLeft = static_cast< int >( std::ceil(extraWidthLeft / dx[0]) );
    int extraGridPointsRight = static_cast<int>( std::ceil(extraWidthRight / dx[0]) );
    int extraGridPointsBottom = static_cast<int>( std::ceil(extraHeightBottom / dx[1]) );
    int extraGridPointsTop = static_cast<int>( std::ceil(extraHeightTop / dx[1]) );

    _idxUpperLeftDomain[0] = _PMLwidth[0] + extraGridPointsLeft;
    _idxUpperLeftDomain[1] = _PMLwidth[1] + extraGridPointsTop;
    _idxLowerRightDomain[0] = _PMLwidth[0] + extraGridPointsLeft + grid.GetGridDimensions()[0];
    _idxLowerRightDomain[1] = _PMLwidth[1] + extraGridPointsTop + grid.GetGridDimensions()[1];

    xMin[0] -= (_PMLwidth[0] + extraGridPointsLeft) * dx[0];
    xMin[1] -= (_PMLwidth[1] + extraGridPointsTop) * dx[1];
    xMax[0] += (_PMLwidth[0] + extraGridPointsRight) * dx[0];
    xMax[1] += (_PMLwidth[1] + extraGridPointsBottom) * dx[1];

    _coordPMLLeft = xMin[0] + _PMLwidth[0] * dx[0];
    _coordPMLRight = xMax[0] - _PMLwidth[0] * dx[0];
    _coordPMLUp = xMin[1] + _PMLwidth[1] * dx[1];
    _coordPMLDown = xMax[1] - _PMLwidth[1] * dx[1];

    std::array<int, 2> nx = {grid.GetGridDimensions()[0] + 2 * _PMLwidth[0] + extraGridPointsLeft + extraGridPointsRight,
                             grid.GetGridDimensions()[1] + 2 * _PMLwidth[1] + extraGridPointsTop + extraGridPointsBottom};

    // Initialize matrix and rhs vector
    _A.resize(nx[0] * nx[1], nx[0] * nx[1]);
    _A.reserve(5 * nx[0] * nx[1]);
    _b.setZero(nx[0] * nx[1]);

    _waveVelocity.resize(nx[0] * nx[1]);
    _newgrid = new FiniteDifferenceGrid2D(xMin, xMax, nx);

    updateChi(chi);
    buildMatrix();
}

Helmholtz2D::~Helmholtz2D()
{
    delete _newgrid;
}

void Helmholtz2D::updateChi(const PressureFieldSerial &chi)
{
    std::array<int, 2> nx = _newgrid->GetGridDimensions();
    std::array<int, 2> oldnx = _oldgrid.GetGridDimensions();
    int indexChiVal, indexWaveVelo, idx1, idx2;

    for (int i = 0; i < nx[0] * nx[1]; ++i)
    {
        _waveVelocity[i] = _c0;
    }

    const double *chiVal = chi.GetDataPtr();
    for (int i = 0; i < oldnx[0]; ++i)
    {
        idx1 = i + _idxUpperLeftDomain[0];
        for (int j = 0; j < oldnx[1]; ++j)
        {
            idx2 = j + _idxUpperLeftDomain[1];
            indexChiVal = j * oldnx[0] + i;
            indexWaveVelo = idx2 * nx[0] + idx1;
            _waveVelocity[indexWaveVelo] = _c0 / sqrt(1. - chiVal[indexChiVal]);
        }
    }
}

PressureFieldComplexSerial Helmholtz2D::solve(const std::array<double, 2> &source, PressureFieldComplexSerial &pInit)
{
    std::array<int, 2> nx = _newgrid->GetGridDimensions();
    std::array<int, 2> oldnx = _oldgrid.GetGridDimensions();

    // Construct vector for this source
    buildVector(source);

    Eigen::VectorXcd result = _solver.solve(_b);
    if (_solver.info() != Eigen::Success)
    {
        L_(lerror) << "Solver failed!" << _solver.info() << " " << _solver.lastErrorMessage() ;
        exit(EXIT_FAILURE);
    }

    int indexpTot, indexResult;
    std::complex<double> *pTot = pInit.GetDataPtr();

    int idx1, idx2;
    for (int i = 0; i < oldnx[0]; ++i)
    {
        idx1 = i + _idxUpperLeftDomain[0];
        for (int j = 0; j < oldnx[1]; ++j)
        {
            idx2 = j + _idxUpperLeftDomain[1];

            indexpTot = j * oldnx[0] + i;
            indexResult = idx2 * nx[0] + idx1;

            pTot[indexpTot] = result[indexResult];
        }
    }

    return pInit;
}

void Helmholtz2D::CreatePMLMatrix(std::vector<Eigen::Triplet<std::complex<double> > > &triplets, std::array<int, 2> nx, double omega, std::array<double, 2> dx, std::array<double, 2> xMin)
{
    std::complex<double> val, Sx, Sz, dSx, dSz;
    double sigmax, sigmaz, nxz, xi, zj;
    int index;
    for (int i = 0; i < nx[0]; ++i) // x index
    {
        xi = xMin[0] + i * dx[0];
        for (int j = 0; j < nx[1]; ++j) // z index
        {
            zj = xMin[1] + j * dx[1];

            index = j * nx[0] + i;
            nxz = 1.0 / _waveVelocity[index];
            if (xi < _coordPMLLeft || xi > _coordPMLRight || zj < _coordPMLUp || zj > _coordPMLDown)
            {
                if (xi < _coordPMLLeft)
                {
                    sigmax = xi - _coordPMLLeft;
                }
                else if (xi > _coordPMLRight)
                {
                    sigmax = xi - _coordPMLRight;
                }
                else
                {
                    sigmax = 0.0;
                }

                if (zj < _coordPMLUp)
                {
                    sigmaz = zj - _coordPMLUp;
                }
                else if (zj > _coordPMLDown)
                {
                    sigmaz = zj - _coordPMLDown;
                }
                else
                {
                    sigmaz = 0.0;
                }

                Sx = 1. + std::complex<double>(0., 1.) * sigmax * sigmax / (omega * dx[0]);
                dSx = -2. * sigmax / (omega * std::complex<double>(0., 1.) * dx[0]);
                Sz = 1. + std::complex<double>(0., 1.) * sigmaz * sigmaz / (omega * dx[1]);
                dSz = -2. * sigmaz / (omega * std::complex<double>(0., 1.) * dx[1]);

                // Diagonal
                val = -2. * Sz / (Sx * dx[0] * dx[0]) - 2. * Sx / (Sz * dx[1] * dx[1]) + Sx * Sz * nxz * nxz * omega * omega;
                triplets.push_back(Eigen::Triplet(index, index, val));

                // Non-diagonal
                if (i != 0)
                {
                    val = Sz / (Sx * dx[0] * dx[0]) + Sz * dSx / (2. * dx[0] * Sx * Sx);
                    triplets.push_back(Eigen::Triplet(index, index - 1, val));
                }
                if (i != nx[0] - 1)
                {
                    val = Sz / (Sx * dx[0] * dx[0]) - Sz * dSx / (2. * dx[0] * Sx * Sx);
                    triplets.push_back(Eigen::Triplet(index, index + 1, val));
                }
                if (j != 0)
                {
                    val = Sx / (Sz * dx[1] * dx[1]) + Sx * dSz / (2. * dx[1] * Sz * Sz);
                    triplets.push_back(Eigen::Triplet(index, index - nx[0], val));
                }
                if (j != nx[1] - 1)
                {
                    val = Sx / (Sz * dx[1] * dx[1]) - Sx * dSz / (2. * dx[1] * Sz * Sz);
                    triplets.push_back(Eigen::Triplet(index, index + nx[0], val));
                }
            }
            else
            {
                // Diagonal
                val = -2. / (dx[0] * dx[0]) - 2. / (dx[1] * dx[1]) + omega * omega * nxz * nxz;
                triplets.push_back(Eigen::Triplet(index, index, val));

                // Non-diagonal
                if (i != 0)
                {
                    val = 1. / (dx[0] * dx[0]);
                    triplets.push_back(Eigen::Triplet(index, index - 1, val));
                }
                if (i != nx[0] - 1)
                {
                    val = 1. / (dx[0] * dx[0]);
                    triplets.push_back(Eigen::Triplet(index, index + 1, val));
                }
                if (j != 0)
                {
                    val = 1. / (dx[1] * dx[1]);
                    triplets.push_back(Eigen::Triplet(index, index - nx[0], val));
                }
                if (j != nx[1] - 1)
                {
                    val = 1. / (dx[1] * dx[1]);
                    triplets.push_back(Eigen::Triplet(index, index + nx[0], val));
                }
            }
        }
    }
}

void Helmholtz2D::CreateABCMatrix(double omega, std::array<double, 2> dx, std::vector<Eigen::Triplet<std::complex<double> > > &triplets, std::array<int, 2> nx)
{
    std::complex<double> val;
    double nxz;
    int index;
    for (int i = 0; i < nx[0]; ++i) // x index
    {
        for (int j = 0; j < nx[1]; ++j) // z index
        {
            index = j * nx[0] + i;
            nxz = 1.0 / _waveVelocity[index];
            //Diagonal
            val = -2. / (dx[0] * dx[0]) - 2. / (dx[1] * dx[1]) + omega * omega * nxz * nxz;
            triplets.push_back(Eigen::Triplet(index, index, val));

            // Non-diagonal
            if (i != 0)
            {
                val = 1. / (dx[0] * dx[0]);
                triplets.push_back(Eigen::Triplet(index, index - 1, val));
            }
            if (i != nx[0] - 1)
            {
                val = 1. / (dx[0] * dx[0]);
                triplets.push_back(Eigen::Triplet(index, index + 1, val));
            }
            if (j != 0)
            {
                val = 1. / (dx[1] * dx[1]);
                triplets.push_back(Eigen::Triplet(index, index - nx[0], val));
            }
            if (j != nx[1] - 1)
            {
                val = 1. / (dx[1] * dx[1]);
                triplets.push_back(Eigen::Triplet(index, index + nx[0], val));
            }

            // ABC
            if (j == 0)
            {
                val = (std::complex(0., 2.) * nxz * omega) / dx[1];
                triplets.push_back(Eigen::Triplet(index, index, val));

                val = 1. / (dx[1] * dx[1]);
                triplets.push_back(Eigen::Triplet(index, index + nx[0], val));
            }
            if (j == nx[1] - 1)
            {
                val = (std::complex(0., 2.) * nxz * omega) / dx[1];
                triplets.push_back(Eigen::Triplet(index, index, val));

                val = 1. / (dx[1] * dx[1]);
                triplets.push_back(Eigen::Triplet(index, index - nx[0], val));
            }
            if (i == 0)
            {
                val = (std::complex(0., 2.) * nxz * omega) / dx[0];
                triplets.push_back(Eigen::Triplet(index, index, val));

                val = 1. / (dx[0] * dx[0]);
                triplets.push_back(Eigen::Triplet(index, index + 1, val));
            }
            if (i == nx[0] - 1)
            {
                val = (std::complex(0., 2.) * nxz * omega) / dx[0];
                triplets.push_back(Eigen::Triplet(index, index, val));

                val = 1. / (dx[0] * dx[0]);
                triplets.push_back(Eigen::Triplet(index, index - 1, val));
            }
        }
    }
}

void Helmholtz2D::buildMatrix()
{
    std::array<int, 2> nx = _newgrid->GetGridDimensions();
    std::array<double, 2> dx = _newgrid->GetMeshSize();
    std::array<double, 2> xMin = _newgrid->GetGridStart();
    double omega = _freq * 2.0 * fwi::pi;

    // Build matrix from new elements
    std::vector<Eigen::Triplet<std::complex<double>>> triplets;
    triplets.reserve(5 * nx[0] * nx[1]); // Naive upper bound for nnz's



    if (_PMLwidth[0] == 0 && _PMLwidth[1] == 0) // 1st Order ABC
    {
        CreateABCMatrix(omega, dx, triplets, nx);
    }
    else // PML
    {
        CreatePMLMatrix(triplets, nx, omega, dx, xMin);
    }


    _A.setFromTriplets(triplets.begin(), triplets.end());
    _A.makeCompressed();

    _solver.analyzePattern(_A);
    _solver.factorize(_A);
    if (_solver.info() != Eigen::Success)
    {
        L_(lerror) << "LU Factorization failed!: " << _solver.info() << " " << _solver.lastErrorMessage() ;
        exit(EXIT_FAILURE);
    }
}

void Helmholtz2D::buildVector(const std::array<double, 2> &source)
{
    std::array<int, 2> nx = _newgrid->GetGridDimensions();
    std::array<double, 2> dx = _newgrid->GetMeshSize();
    std::array<double, 2> xMin = _newgrid->GetGridStart();

    // Reset vector to zero
    _b.setZero(nx[0] * nx[1]);

    if (_srcInput.r == 0)
    {
        // Add point source to the nearest grid point
        std::array<double, 2> originalxMin = _oldgrid.GetGridStart();
        int i = _idxUpperLeftDomain[0] + std::lround((source[0] - originalxMin[0]) / dx[0]);
        int j = _idxUpperLeftDomain[1] + std::lround((source[1] - originalxMin[1]) / dx[1]);
        _b[j * nx[0] + i] = 1. / (dx[0] * dx[1]);
    }
    else
    {
        /* Use Sine source function with a Kaiser window function
         * NOTE: This requires r extra grid points added around each source and hence a larger grid */
        double xi, zj, nxdist, nzdist, Wx, fx, Wz, fz;
        int index;
        double r = static_cast<double>(_srcInput.r);

        for (int i = 0; i < nx[0]; ++i)
        {
            xi = xMin[0] + i * dx[0];
            nxdist = abs(source[0] - xi) / dx[0];

            for (int j = 0; j < nx[1]; ++j)
            {
                zj = xMin[1] + j * dx[1];
                nzdist = abs(source[1] - zj) / dx[1];

                if (nxdist < r && nzdist < r)
                {
                    Wx = std::cyl_bessel_i(0., _srcInput.beta * sqrt(1. - (nxdist * nxdist) / (r * r))) / std::cyl_bessel_i(0., _srcInput.beta);
                    Wz = std::cyl_bessel_i(0., _srcInput.beta * sqrt(1. - (nzdist * nzdist) / (r * r))) / std::cyl_bessel_i(0., _srcInput.beta);

                    // Sine source function
                    if (nxdist > 0.0)
                    {
                        fx = Wx * sin(fwi::pi * nxdist) / (dx[0] * fwi::pi * nxdist);
                    }
                    else
                    {
                        fx = Wx / dx[0];
                    }

                    if (nzdist > 0.0)
                    {
                        fz = Wz * sin(fwi::pi * nzdist) / (dx[1] * fwi::pi * nzdist);
                    }
                    else
                    {
                        fz = Wz / dx[1];
                    }

                    index = j * nx[0] + i;
                    _b[index] = fx * fz;
                }
            }
        }
    }
}
