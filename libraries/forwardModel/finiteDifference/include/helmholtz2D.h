#pragma once

#include "Eigen/Sparse"
#include "grid2D.h"
#include "finiteDifferenceGrid2D.h"
#include "frequenciesGroup.h"
#include "pressureFieldComplexSerial.h"
#include "sources.h"
#include "finiteDifferenceForwardModelInput.h"

#include <complex>
#include <vector>
#include <string>

class Helmholtz2D {
  public:
    explicit Helmholtz2D(const Grid2D &grid, const double freq, const Sources &src, const double c0, const PressureFieldSerial &chi, const FiniteDifferenceForwardModelInput &fmInput);
    ~Helmholtz2D();
    PressureFieldComplexSerial solve(const std::array<double, 2> &source, PressureFieldComplexSerial &pInit);
  private:
    Eigen::SparseMatrix<std::complex<double>> _A;
    Eigen::VectorXcd _b;
    const Grid2D _oldgrid;
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
    void updateChi(const PressureFieldSerial &chi);
};
