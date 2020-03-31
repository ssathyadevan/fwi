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

class PressureFieldComplexSerial : public PressureFieldComplex
{
private:
    std::vector<std::complex<double>> _data;
    std::complex<double> *const _dataPointer;

public:
    PressureFieldComplexSerial(const Grid2D &grid);
    PressureFieldComplexSerial(const PressureFieldComplexSerial &rhs);

    // Virtual overrides
    void Zero() override;
    void Square() override;
    void Reciprocal() override;
    virtual void Conjugate();

    double Norm() const override { return std::sqrt(std::real(InnerProduct(*this))); }
    double RelNorm() const override { return std::sqrt(std::real(InnerProduct(*this))) / GetNumberOfGridPoints(); }
    std::complex<double> Summation() const override;

    void SetField(const std::function<std::complex<double>(double, double)> func) override;

    void toBuffer(std::complex<double> *buffer) const override;
    void fromBuffer(const std::complex<double> *buffer) override;

    void toFile(const std::string &fileName) const override;
    void fromFile(const std::string &fileName) override;

    // Non virtual members
    const std::vector<std::complex<double>> &getData() const { return _data; }

    void setData(const std::vector<std::complex<double>> data);
    void setValueAtIndex(const std::complex<double> value, const int index) { _data[index] = value; }
    void addValueAtIndex(const std::complex<double> value, const int index) { _data[index] += value; }

    std::complex<double> InnerProduct(const PressureFieldComplexSerial &rhs) const;
    std::complex<double> DotProduct(const PressureFieldComplexSerial &rhs) const;
    std::complex<double> DotProduct(const PressureFieldSerial &rhs) const;

    PressureFieldSerial GetRealPart() const;

    virtual ~PressureFieldComplexSerial() {}

    virtual void Random();

    // TODO: REMOVE
    const std::complex<double> *GetDataPtr() const { return _dataPointer; }
    std::complex<double> *GetDataPtr() { return _dataPointer; }

    PressureFieldComplexSerial &operator=(const PressureFieldComplexSerial &rhs);
    PressureFieldComplexSerial &operator=(const PressureFieldSerial &rhs);
    PressureFieldComplexSerial &operator=(const std::complex<double> rhs);
    PressureFieldComplexSerial &operator=(const double rhs);

    PressureFieldComplexSerial &operator+=(const PressureFieldComplexSerial &rhs);
    PressureFieldComplexSerial &operator+=(const std::complex<double> rhs);
    PressureFieldComplexSerial &operator+=(const double rhs);

    PressureFieldComplexSerial &operator-=(const PressureFieldComplexSerial &rhs);
    PressureFieldComplexSerial &operator-=(const std::complex<double> rhs);
    PressureFieldComplexSerial &operator-=(const double rhs);

    PressureFieldComplexSerial &operator*=(const PressureFieldComplexSerial &rhs);
    PressureFieldComplexSerial &operator*=(const PressureFieldSerial &rhs);
    PressureFieldComplexSerial &operator*=(const std::complex<double> rhs);
    PressureFieldComplexSerial &operator*=(const double rhs);

    PressureFieldComplexSerial &operator/=(const PressureFieldComplexSerial &rhs);
    PressureFieldComplexSerial &operator/=(const PressureFieldSerial &rhs);
    PressureFieldComplexSerial &operator/=(const std::complex<double> rhs);
    PressureFieldComplexSerial &operator/=(const double rhs);
};

inline std::complex<double> InnerProduct(const PressureFieldComplexSerial &t1, const PressureFieldComplexSerial &t2) { return t1.InnerProduct(t2); }

inline std::complex<double> DotProduct(const PressureFieldComplexSerial &t1, const PressureFieldComplexSerial &t2) { return t1.DotProduct(t2); }

inline std::complex<double> DotProduct(const PressureFieldComplexSerial &t1, const PressureFieldSerial &t2) { return t1.DotProduct(t2); }

inline PressureFieldComplexSerial operator-(const PressureFieldComplexSerial &t1, const PressureFieldComplexSerial &t2)
{
    PressureFieldComplexSerial t3(t1);
    t3 -= t2;
    return t3;
}

inline PressureFieldComplexSerial operator*(const PressureFieldComplexSerial &t1, const PressureFieldSerial &t2)
{
    PressureFieldComplexSerial t3(t1);
    t3 *= t2;
    return t3;
}

inline PressureFieldComplexSerial operator*(const PressureFieldComplexSerial &t1, const PressureFieldComplexSerial &t2)
{
    PressureFieldComplexSerial t3(t1);
    t3 *= t2;
    return t3;
}

inline PressureFieldComplexSerial operator*(const PressureFieldSerial &t1, const PressureFieldComplexSerial &t2) { return t2 * t1; }

inline PressureFieldComplexSerial operator*(const PressureFieldComplexSerial &t1, const std::complex<double> t2)
{
    PressureFieldComplexSerial t3(t1);
    t3 *= t2;
    return t3;
}

inline PressureFieldComplexSerial operator*(const std::complex<double> t1, const PressureFieldComplexSerial &t2) { return t2 * t1; }

inline PressureFieldComplexSerial operator/(const PressureFieldComplexSerial &t1, const double t2)
{
    PressureFieldComplexSerial t3(t1);
    t3 /= t2;
    return t3;
}
