#pragma once

#include <complex>
#include <vector>
#include <string>

#include "Eigen/Sparse"
#include "grid2D.h"
#include "finiteDifferenceGrid2D.h"
#include "frequenciesGroup.h"
#include "pressureFieldComplexSerial.h"
#include "sources.h"
#include "forwardModelInput.h"

class Helmholtz2D {
public:
    explicit Helmholtz2D(const grid2D &grid, const double freq, const sources &src, const double c0, const pressureFieldSerial &chi, const forwardModelInput &fmInput);
    ~Helmholtz2D();
    pressureFieldComplexSerial Solve(const std::array<double, 2> &source, pressureFieldComplexSerial &pInit);
private:
    Eigen::SparseMatrix<std::complex<double>> _A;
    Eigen::VectorXcd _b;
    const grid2D _oldgrid;
    FiniteDifferenceGrid2D *_newgrid;
    std::array<int, 2> _PMLwidth;
    std::array<int, 2> _idxUpperLeftDomain, _idxLowerRightDomain;
    double _coordPMLLeft, _coordPMLRight, _coordPMLUp, _coordPMLDown;
    const double _freq;
    double _c0;
    std::vector<double> _waveVelocity;
    Eigen::SparseLU<Eigen::SparseMatrix<std::complex<double>>, Eigen::NaturalOrdering<int>> _solver;
    const SourceParameter _srcInput;

    void BuildMatrix();
    void BuildVector(const std::array<double, 2> &source);
    void updateChi(const pressureFieldSerial &chi);
};
