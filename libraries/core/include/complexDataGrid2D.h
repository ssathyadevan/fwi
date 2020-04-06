#pragma once

#include "grid2D.h"
#include "dataGrid2D.h"
#include <array>
#include <cassert>
#include <complex>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

class complexDataGrid2D
{
private:
    const grid2D &_grid;
    std::vector<std::complex<double>> _data;

public:
    explicit complexDataGrid2D(const grid2D &grid);
    complexDataGrid2D(const complexDataGrid2D &rhs);
    ~complexDataGrid2D() {}

    const std::vector<std::complex<double>> &getData() const { return _data; }
    void setValueAtIndex(const std::complex<double> value, const int index) { _data[index] = value; }
    void addValueAtIndex(const std::complex<double> value, const int index) { _data[index] += value; }

    const grid2D &getGrid() const { return _grid; }
    int getNumberOfGridPoints() const { return _grid.getNumberOfGridPoints(); }
    double getCellVolume() const { return _grid.getCellVolume(); }

    void zero();
    void square();
    void sqrt();
    void reciprocal();
    void conjugate();

    double norm() const { return std::sqrt(innerProduct(*this)); }
    double relNorm() const { return std::sqrt(innerProduct(*this) / getNumberOfGridPoints()); }
    std::complex<double> Summation() const;

    double innerProduct(const complexDataGrid2D &rhs) const;
    std::complex<double> dotProduct(const complexDataGrid2D &rhs) const;
    std::complex<double> dotProduct(const dataGrid2D &rhs) const;

    dataGrid2D getRealPart() const;

    void toFile(const std::string &fileName) const;
    void fromFile(const std::string &fileName);

    void Random();

    // Operators
    complexDataGrid2D &operator=(const complexDataGrid2D &rhs);
    complexDataGrid2D &operator=(const dataGrid2D &rhs);
    complexDataGrid2D &operator=(const std::vector<std::complex<double>> &rhs);
    complexDataGrid2D &operator=(const std::vector<double> &rhs);
    complexDataGrid2D &operator=(const std::complex<double> rhs);
    complexDataGrid2D &operator=(const double rhs);

    complexDataGrid2D &operator+=(const complexDataGrid2D &rhs);
    complexDataGrid2D &operator+=(const dataGrid2D &rhs);
    complexDataGrid2D &operator+=(const std::vector<std::complex<double>> &rhs);
    complexDataGrid2D &operator+=(const std::vector<double> &rhs);
    complexDataGrid2D &operator+=(const std::complex<double> rhs);
    complexDataGrid2D &operator+=(const double rhs);

    complexDataGrid2D &operator-=(const complexDataGrid2D &rhs);
    complexDataGrid2D &operator-=(const dataGrid2D &rhs);
    complexDataGrid2D &operator-=(const std::vector<std::complex<double>> &rhs);
    complexDataGrid2D &operator-=(const std::vector<double> &rhs);
    complexDataGrid2D &operator-=(const std::complex<double> rhs);
    complexDataGrid2D &operator-=(const double rhs);

    complexDataGrid2D &operator*=(const complexDataGrid2D &rhs);
    complexDataGrid2D &operator*=(const dataGrid2D &rhs);
    complexDataGrid2D &operator*=(const std::vector<std::complex<double>> &rhs);
    complexDataGrid2D &operator*=(const std::vector<double> &rhs);
    complexDataGrid2D &operator*=(const std::complex<double> rhs);
    complexDataGrid2D &operator*=(const double rhs);

    complexDataGrid2D &operator/=(const complexDataGrid2D &rhs);
    complexDataGrid2D &operator/=(const dataGrid2D &rhs);
    complexDataGrid2D &operator/=(const std::vector<std::complex<double>> &rhs);
    complexDataGrid2D &operator/=(const std::vector<double> &rhs);
    complexDataGrid2D &operator/=(const std::complex<double> rhs);
    complexDataGrid2D &operator/=(const double rhs);
};

inline double innerProduct(const complexDataGrid2D &t1, const complexDataGrid2D &t2) { return t1.innerProduct(t2); }

inline std::complex<double> dotProduct(const complexDataGrid2D &t1, const complexDataGrid2D &t2) { return t1.dotProduct(t2); }

inline std::complex<double> dotProduct(const complexDataGrid2D &t1, const dataGrid2D &t2) { return t1.dotProduct(t2); }

inline complexDataGrid2D operator-(const complexDataGrid2D &t1, const complexDataGrid2D &t2)
{
    complexDataGrid2D t3(t1);
    t3 -= t2;
    return t3;
}

inline complexDataGrid2D operator*(const complexDataGrid2D &t1, const dataGrid2D &t2)
{
    complexDataGrid2D t3(t1);
    t3 *= t2;
    return t3;
}

inline complexDataGrid2D operator*(const complexDataGrid2D &t1, const complexDataGrid2D &t2)
{
    complexDataGrid2D t3(t1);
    t3 *= t2;
    return t3;
}

inline complexDataGrid2D operator*(const dataGrid2D &t1, const complexDataGrid2D &t2) { return t2 * t1; }

inline complexDataGrid2D operator*(const complexDataGrid2D &t1, const std::complex<double> t2)
{
    complexDataGrid2D t3(t1);
    t3 *= t2;
    return t3;
}

inline complexDataGrid2D operator*(const std::complex<double> t1, const complexDataGrid2D &t2) { return t2 * t1; }

inline complexDataGrid2D operator/(const complexDataGrid2D &t1, const double t2)
{
    complexDataGrid2D t3(t1);
    t3 /= t2;
    return t3;
}
