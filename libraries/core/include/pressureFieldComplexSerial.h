#pragma once

#include "grid2D.h"
#include "pressureFieldComplex.h"
#include "pressureFieldSerial.h"
#include <array>
#include <cassert>
#include <complex>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

class pressureFieldComplexSerial : public pressureFieldComplex
{
private:
    std::vector<std::complex<double>> _data;
    std::complex<double> *const _dataPointer;

public:
    explicit pressureFieldComplexSerial(const grid2D &grid);
    pressureFieldComplexSerial(const pressureFieldComplexSerial &rhs);
    ~pressureFieldComplexSerial() {}

    // Virtual overrides
    void zero() override;
    void square() override;
    void sqrt() override;
    void reciprocal() override;

    double norm() const override { return std::sqrt(innerProduct(*this)); }
    double relNorm() const override { return std::sqrt(innerProduct(*this) / getNumberOfGridPoints()); }
    std::complex<double> Summation() const override;

    void toBuffer(std::complex<double> *buffer) const override;
    void fromBuffer(const std::complex<double> *buffer) override;

    void toFile(const std::string &fileName) const override;
    void fromFile(const std::string &fileName) override;

    // Non virtual members
    const std::vector<std::complex<double>> &getData() const { return _data; }
    void setValueAtIndex(const std::complex<double> value, const int index) { _data[index] = value; }
    void addValueAtIndex(const std::complex<double> value, const int index) { _data[index] += value; }

    void conjugate();

    double innerProduct(const pressureFieldComplexSerial &rhs) const;
    std::complex<double> dotProduct(const pressureFieldComplexSerial &rhs) const;
    std::complex<double> dotProduct(const pressureFieldSerial &rhs) const;

    pressureFieldSerial getRealPart() const;

    void Random();

    // TODO: REMOVE
    const std::complex<double> *getDataPtr() const { return _dataPointer; }
    std::complex<double> *getDataPtr() { return _dataPointer; }

    pressureFieldComplexSerial &operator=(const pressureFieldComplexSerial &rhs);          // check
    pressureFieldComplexSerial &operator=(const pressureFieldSerial &rhs);                 // check
    pressureFieldComplexSerial &operator=(const std::vector<std::complex<double>> &rhs);   // check
    pressureFieldComplexSerial &operator=(const std::vector<double> &rhs);                 // check
    pressureFieldComplexSerial &operator=(const std::complex<double> rhs);                 // check
    pressureFieldComplexSerial &operator=(const double rhs);                               // check

    pressureFieldComplexSerial &operator+=(const pressureFieldComplexSerial &rhs);
    pressureFieldComplexSerial &operator+=(const pressureFieldSerial &rhs);
    pressureFieldComplexSerial &operator+=(const std::vector<std::complex<double>> &rhs);
    pressureFieldComplexSerial &operator+=(const std::vector<double> &rhs);
    pressureFieldComplexSerial &operator+=(const std::complex<double> rhs);
    pressureFieldComplexSerial &operator+=(const double rhs);

    pressureFieldComplexSerial &operator-=(const pressureFieldComplexSerial &rhs);
    pressureFieldComplexSerial &operator-=(const pressureFieldSerial &rhs);
    pressureFieldComplexSerial &operator-=(const std::vector<std::complex<double>> &rhs);
    pressureFieldComplexSerial &operator-=(const std::vector<double> &rhs);
    pressureFieldComplexSerial &operator-=(const std::complex<double> rhs);
    pressureFieldComplexSerial &operator-=(const double rhs);

    pressureFieldComplexSerial &operator*=(const pressureFieldComplexSerial &rhs);
    pressureFieldComplexSerial &operator*=(const pressureFieldSerial &rhs);
    pressureFieldComplexSerial &operator*=(const std::vector<std::complex<double>> &rhs);
    pressureFieldComplexSerial &operator*=(const std::vector<double> &rhs);
    pressureFieldComplexSerial &operator*=(const std::complex<double> rhs);
    pressureFieldComplexSerial &operator*=(const double rhs);

    pressureFieldComplexSerial &operator/=(const pressureFieldComplexSerial &rhs);
    pressureFieldComplexSerial &operator/=(const pressureFieldSerial &rhs);
    pressureFieldComplexSerial &operator/=(const std::vector<std::complex<double>> &rhs);
    pressureFieldComplexSerial &operator/=(const std::vector<double> &rhs);
    pressureFieldComplexSerial &operator/=(const std::complex<double> rhs);
    pressureFieldComplexSerial &operator/=(const double rhs);
};

inline double InnerProduct(const pressureFieldComplexSerial &t1, const pressureFieldComplexSerial &t2) { return t1.innerProduct(t2); }

inline std::complex<double> DotProduct(const pressureFieldComplexSerial &t1, const pressureFieldComplexSerial &t2) { return t1.dotProduct(t2); }

inline std::complex<double> DotProduct(const pressureFieldComplexSerial &t1, const pressureFieldSerial &t2) { return t1.dotProduct(t2); }

inline pressureFieldComplexSerial operator-(const pressureFieldComplexSerial &t1, const pressureFieldComplexSerial &t2)
{
    pressureFieldComplexSerial t3(t1);
    t3 -= t2;
    return t3;
}

inline pressureFieldComplexSerial operator*(const pressureFieldComplexSerial &t1, const pressureFieldSerial &t2)
{
    pressureFieldComplexSerial t3(t1);
    t3 *= t2;
    return t3;
}

inline pressureFieldComplexSerial operator*(const pressureFieldComplexSerial &t1, const pressureFieldComplexSerial &t2)
{
    pressureFieldComplexSerial t3(t1);
    t3 *= t2;
    return t3;
}

inline pressureFieldComplexSerial operator*(const pressureFieldSerial &t1, const pressureFieldComplexSerial &t2) { return t2 * t1; }

inline pressureFieldComplexSerial operator*(const pressureFieldComplexSerial &t1, const std::complex<double> t2)
{
    pressureFieldComplexSerial t3(t1);
    t3 *= t2;
    return t3;
}

inline pressureFieldComplexSerial operator*(const std::complex<double> t1, const pressureFieldComplexSerial &t2) { return t2 * t1; }

inline pressureFieldComplexSerial operator/(const pressureFieldComplexSerial &t1, const double t2)
{
    pressureFieldComplexSerial t3(t1);
    t3 /= t2;
    return t3;
}
