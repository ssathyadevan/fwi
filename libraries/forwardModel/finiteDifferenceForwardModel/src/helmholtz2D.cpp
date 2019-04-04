#include "helmholtz2D.h"

using namespace Eigen;

Helmholtz2D::Helmholtz2D(const grid2D &grid, const double freq, const sources &src, const double c0, const pressureFieldSerial &chi, const PMLWidthFactor &pmlFactor)
    : _A(), _b(), _oldgrid(grid), _newgrid(), _PMLwidth(), _freq(freq), _c0(c0), _waveVelocity(), _solver()
{    
    double waveLength = _c0/freq;

    //calculating the width of the perfectly matching layer. (rounded up)
    std::array<double, 2> cellDimensions = grid.GetCellDimensions();

    _PMLwidth[0] = std::round(pmlFactor.x*waveLength/cellDimensions[0] + 0.5);
    _PMLwidth[1] = std::round(pmlFactor.z*waveLength/cellDimensions[1] + 0.5);

    std::array<double, 2> xMin = grid.GetGridStart();
    std::array<double, 2> xMax = grid.GetGridEnd();

    //sources can be outside imaging domain. Area that we solve for needs to include all sources
    double extraWidthLeft = 0.0;
    double extraWidthRight = 0.0;
    double extraWidthBottom = 0.0;
    double extraWidthTop = 0.0;

    for (int i = 0; i < src.nSrc; i++)
    {
        double x = (src.xSrc[i])[0];
        double z = (src.xSrc[i])[1];

        if ( xMin[0]-x > extraWidthLeft )
            extraWidthLeft = xMin[0] - x;
        else if ( x-xMax[0] > extraWidthRight )
            extraWidthRight = x - xMax[0];
        if ( xMin[1]-z > extraWidthTop )
            extraWidthTop = xMin[1] - z;
        else if ( z-xMax[1] > extraWidthBottom )
            extraWidthBottom = z - xMax[1];
    }

    int extraGridPointsLeft   = std::ceil( extraWidthLeft   / cellDimensions[0] );
    int extraGridPointsRight  = std::ceil( extraWidthRight  / cellDimensions[0] );
    int extraGridPointsBottom = std::ceil( extraWidthBottom / cellDimensions[1] );
    int extraGridPointsTop    = std::ceil( extraWidthTop    / cellDimensions[1] );

    _idxUpperLeftDomain[0]  = _PMLwidth[0] + extraGridPointsLeft;
    _idxUpperLeftDomain[1]  = _PMLwidth[1] + extraGridPointsTop;
    _idxLowerRightDomain[0] = _PMLwidth[0] + extraGridPointsLeft + grid.GetGridDimensions()[0];
    _idxLowerRightDomain[1] = _PMLwidth[1] + extraGridPointsTop  + grid.GetGridDimensions()[1];

    xMin[0] -= (_PMLwidth[0] + extraGridPointsLeft  ) * cellDimensions[0];
    xMin[1] -= (_PMLwidth[1] + extraGridPointsTop   ) * cellDimensions[1];
    xMax[0] += (_PMLwidth[0] + extraGridPointsRight ) * cellDimensions[0];
    xMax[1] += (_PMLwidth[1] + extraGridPointsBottom) * cellDimensions[1];

    std::array<int, 2> nx = { grid.GetGridDimensions()[0] + 2*_PMLwidth[0] + extraGridPointsLeft + extraGridPointsRight,
                              grid.GetGridDimensions()[1] + 2*_PMLwidth[1] + extraGridPointsTop + extraGridPointsBottom };

    // Initialize matrix and rhs vector
    _A.resize(nx[0]*nx[1], nx[0]*nx[1]);
    _A.reserve(5*nx[0]*nx[1]);
    _b.setZero(nx[0]*nx[1]);

    _waveVelocity.resize(nx[0]*nx[1]);
    _newgrid = new FiniteDifferenceGrid2D(xMin, xMax, nx);

    updateChi(chi);
    BuildMatrix();
}

Helmholtz2D::~Helmholtz2D()
{
    delete _newgrid;
}

void Helmholtz2D::updateChi(const pressureFieldSerial &chi)
{
    std::array<int, 2> nx = _newgrid->GetGridDimensions();
    std::array<int, 2> oldnx = _oldgrid.GetGridDimensions();
    int indexChiVal, indexWaveVelo, idx1, idx2;

    for(int i = 0; i < nx[0]*nx[1]; ++i)
    {
        _waveVelocity[i] = _c0;
    }

    const double* chiVal = chi.GetDataPtr();
    for (int i = 0; i < oldnx[0]; ++i)
    {
        idx1 = i + _idxUpperLeftDomain[0];
        for (int j = 0; j < oldnx[1]; ++j)
        {
            idx2 = j + _idxUpperLeftDomain[1];
            indexChiVal = j*oldnx[0] + i;
            indexWaveVelo = idx2*nx[0] + idx1;
            _waveVelocity[indexWaveVelo] = _c0 / sqrt(1. - chiVal[indexChiVal]);
        }
    }
}

pressureFieldComplexSerial Helmholtz2D::Solve(const std::array<double, 2> &source, pressureFieldComplexSerial &pInit)
{
    std::array<int, 2> nx = _newgrid->GetGridDimensions();
    std::array<int, 2> oldnx = _oldgrid.GetGridDimensions();
    std::array<double, 2> dx = _newgrid->GetMeshSize();
    std::array<double, 2> originalxMin = _oldgrid.GetGridStart();

    int xi = _idxUpperLeftDomain[0] + std::round((source[0] - originalxMin[0])/dx[0]);
    int zj = _idxUpperLeftDomain[1] + std::round((source[1] - originalxMin[1])/dx[1]);

    // Construct vector for this source
    _b[zj*nx[0]+xi] = 1. / (dx[0]*dx[1]);

    VectorXcd result = _solver.solve(_b);
    if (_solver.info() != Success) {
        std::cout << "Solver failed!" << _solver.info() << " " << _solver.lastErrorMessage() << std::endl;
        exit(EXIT_FAILURE);
    }

    int indexpTot, indexResult;
    std::complex<double>* pTot = pInit.GetDataPtr();

    int idx1, idx2;
    for (int i = 0; i < oldnx[0]; ++i)
    {
        idx1 = i + _idxUpperLeftDomain[0];
        for (int j = 0; j < oldnx[1]; ++j)
        {
            idx2 = j + _idxUpperLeftDomain[1];

            indexpTot = j*oldnx[0] + i;
            indexResult = idx2*nx[0] + idx1;

            pTot[indexpTot] = result[indexResult];
        }
    }

    _b[zj*nx[0]+xi] = 0.0;

    return pInit;
}

void Helmholtz2D::BuildMatrix()
{
    std::array<int, 2> nx = _newgrid->GetGridDimensions();
    std::array<double, 2> dx = _newgrid->GetMeshSize();
    double omega = _freq * 2.0 * M_PI;

    // Build matrix from new elements
    std::vector<Triplet<std::complex<double>>> triplets;
    triplets.reserve(5*nx[0]*nx[1]); // Naive upper bound for nnz's

    std::complex<double> val, Sx, Sz, dSx, dSz;
    double sigmax, sigmaz, nxz;
    int index;
    for (int i = 0; i < nx[0]; ++i) // x index
    {
        for (int j = 0; j < nx[1]; ++j) // z index
        {
            index = j * nx[0] + i;
            nxz = 1.0/_waveVelocity[index];

            // Figure out if in PML or not
            if (i < _PMLwidth[0] || i >= nx[0]-_PMLwidth[0] || j < _PMLwidth[1] || j >= nx[1]-_PMLwidth[1])
            {
                if (i < _PMLwidth[0]) {
                    sigmax = -(_PMLwidth[0] - i) * (dx[0]);
                } else if (i >= nx[0] - _PMLwidth[0]) {
                    sigmax = ( i+1 - (nx[0] - _PMLwidth[0]) ) * (dx[0]);
                } else {
                    sigmax = 0.0;
                }

                if (j < _PMLwidth[1]) {
                    sigmaz = -(_PMLwidth[1] - j)*(dx[1]);
                } else if (j >= nx[1] - _PMLwidth[1]) {
                    sigmaz = ( j+1 - (nx[1] - _PMLwidth[1]) ) * (dx[1]);
                } else {
                    sigmaz = 0.0;
                }

                Sx = 1. - std::complex<double>(0.,1.) * sigmax * sigmax / omega;
                dSx = 2. * sigmax / (omega*std::complex<double>(0.,1.));
                Sz = 1. - std::complex<double>(0.,1.) * sigmaz * sigmaz / omega;
                dSz = 2. * sigmaz / (omega*std::complex<double>(0.,1.));

                // Diagonal
                val = -2. * Sz / (Sx*dx[0]*dx[0]) - 2. * Sx / (Sz*dx[1]*dx[1]) + Sx*Sz*nxz*nxz*omega*omega;
                triplets.push_back(Triplet(index,index,val));

                // Non-diagonal
                if (i != 0) {
                    val = Sz / (Sx*dx[0]*dx[0]) + Sz * dSx / (2.*dx[0]*Sx*Sx);
                    triplets.push_back(Triplet(index,index-1,val));
                }
                if (i != nx[0]-1) {
                    val = Sz / (Sx*dx[0]*dx[0]) - Sz * dSx / (2.*dx[0]*Sx*Sx);
                    triplets.push_back(Triplet(index,index+1,val));
                }
                if (j != 0) {
                    val = Sx / (Sz*dx[1]*dx[1]) + Sx * dSz / (2.*dx[1]*Sz*Sz);
                    triplets.push_back(Triplet(index,index-nx[0],val));
                }
                if (j != nx[1]-1) {
                    val = Sx / (Sz*dx[1]*dx[1]) - Sx * dSz / (2.*dx[1]*Sz*Sz);
                    triplets.push_back(Triplet(index,index+nx[0],val));
                }
            }
            else
            {
                // Diagonal
                val = -2. / (dx[0]*dx[0]) - 2. / (dx[1]*dx[1]) + omega*omega *nxz*nxz;
                triplets.push_back(Triplet(index,index,val));

                // Non-diagonal
                if (i != 0) {
                    val = 1. / (dx[0]*dx[0]);
                    triplets.push_back(Triplet(index,index-1,val));
                }
                if (i != nx[0]-1) {
                    val = 1. /(dx[0]*dx[0]);
                    triplets.push_back(Triplet(index,index+1,val));
                }
                if (j != 0) {
                    val = 1. / (dx[1]*dx[1]);
                    triplets.push_back(Triplet(index,index-nx[0],val));
                }
                if (j != nx[1]-1) {
                    val = 1. / (dx[1]*dx[1]);
                    triplets.push_back(Triplet(index,index+nx[0],val));
                }
            }
        }
    }

    _A.setFromTriplets(triplets.begin(), triplets.end());
    _A.makeCompressed();

    _solver.analyzePattern(_A);
    _solver.factorize(_A);
    if (_solver.info() != Success) {
        std::cout << "LU Factorization failed!: " << _solver.info() << " " << _solver.lastErrorMessage() << std::endl;
        exit(EXIT_FAILURE);
    }
}
