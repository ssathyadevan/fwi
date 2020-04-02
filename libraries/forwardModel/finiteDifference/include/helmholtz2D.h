#pragma once

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif // _MSC_VER
#include "Eigen/Sparse"

#ifdef _MSC_VER
#pragma warning( pop )
#endif // _MSC_VER

#include "grid2D.h"
#include "finiteDifferenceGrid2D.h"
#include "frequenciesGroup.h"
#include "complexDataGrid2D.h"
#include "sources.h"
#include "finiteDifferenceForwardModelInput.h"

#include <complex>
#include <vector>
#include <string>

class Helmholtz2D {
  public:
    explicit Helmholtz2D(const grid2D &grid, const double freq, const sources &src, const double c0, const dataGrid2D &chi, const finiteDifferenceForwardModelInput &fmInput);
    ~Helmholtz2D();
    complexDataGrid2D solve(const std::array<double, 2> &source, complexDataGrid2D &pInit);
    void CreateABCMatrix(double omega, std::array<double, 2> dx, std::vector<Eigen::Triplet<std::complex<double>>>& triplets, std::array<int, 2> nx); //Temporary for testing

private:
    Eigen::SparseMatrix<std::complex<double>> _A;
    Eigen::VectorXcd _b;
    const grid2D _oldgrid;
    FiniteDifferenceGrid2D *_newgrid;
    std::array<int, 2> _PMLwidth;
    std::array<int, 2> _idxUpperLeftDomain, _idxLowerRightDomain;
    double _coordPMLLeft, _coordPMLRight, _coordPMLUp, _coordPMLDown;
    const double _freq;
    const double _c0;
    std::vector<double> _waveVelocity;
    Eigen::SparseLU<Eigen::SparseMatrix<std::complex<double>>, Eigen::NaturalOrdering<int>> _solver;
    const SourceParameter _srcInput;

    void buildMatrix();
    void buildVector(const std::array<double, 2> &source);
    void updateChi(const dataGrid2D &chi);
    void CreatePMLMatrix(std::vector<Eigen::Triplet<std::complex<double>>> &triplets, std::array<int, 2> nx, double omega, std::array<double, 2> dx, std::array<double, 2> xMin);
};
