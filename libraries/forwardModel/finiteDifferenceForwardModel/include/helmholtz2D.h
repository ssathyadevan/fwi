#pragma once

#include <complex>
#include <vector>
#include <string>

#include "Eigen/Sparse"
#include "grid2D.h"
#include "frequenciesGroup.h"
#include "pressureFieldComplexSerial.h"
#include "sources.h"


class Helmholtz2D {
public:
    explicit Helmholtz2D(const grid2D &grid, const double freq, const sources &src, const double c0, const pressureFieldSerial &chi);
    ~Helmholtz2D();
    pressureFieldComplexSerial Solve(const std::array<double, 2> &source, pressureFieldComplexSerial &pInit);
private:
    Eigen::SparseMatrix<std::complex<double>> _A;
    Eigen::VectorXcd _b;
    const grid2D _oldgrid;
    grid2D *_newgrid;
    std::array<int, 2> _PMLwidth;
    std::array<int, 2> _idxUpperLeftDomain, _idxLowerRightDomain;
    const double _freq;
    double _c0;
    std::vector<double> _waveVelocity;
    Eigen::SparseLU<Eigen::SparseMatrix<std::complex<double>>, Eigen::NaturalOrdering<int>> _solver;

    void BuildMatrix();
    void updateChi(const pressureFieldSerial &chi);
};
