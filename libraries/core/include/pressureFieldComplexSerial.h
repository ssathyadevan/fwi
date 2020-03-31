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

    virtual ~PressureFieldComplexSerial();

    virtual void SetField(const std::function<std::complex<double>(double, double)> func);

    virtual void toBuffer(std::complex<double> *buffer) const;

    virtual void fromBuffer(const std::complex<double> *buffer);

    virtual void toFile(const std::string &fileName) const;

    virtual void fromFile(const std::string &fileName);

    virtual void Zero();

    virtual void Square();

    virtual void Reciprocal();

    virtual void Conjugate();

    virtual void Random();

    virtual double Norm() const;

    virtual double RelNorm() const;

    void setData(const std::vector<std::complex<double>> data);

    const std::vector<std::complex<double>> &getData() const { return _data; }

    const std::complex<double> *GetDataPtr() const { return _dataPointer; }

    std::complex<double> *GetDataPtr() { return _dataPointer; }

    PressureFieldComplexSerial &operator=(const PressureFieldComplexSerial &rhs);

    PressureFieldComplexSerial &operator=(const PressureFieldSerial &rhs);

    PressureFieldComplexSerial &operator=(const double &rhs);

    PressureFieldComplexSerial &operator+=(const PressureFieldComplexSerial &rhs);

    PressureFieldComplexSerial &operator-=(const PressureFieldComplexSerial &rhs);

    PressureFieldComplexSerial &operator*=(const PressureFieldComplexSerial &rhs);

    PressureFieldComplexSerial &operator*=(const PressureFieldSerial &rhs);

    PressureFieldComplexSerial &operator+=(const double &rhs);

    PressureFieldComplexSerial &operator-=(const double &rhs);

    PressureFieldComplexSerial &operator*=(const double &rhs);

    PressureFieldComplexSerial &operator/=(const double &rhs);

    PressureFieldComplexSerial &operator+=(const std::complex<double> &rhs);

    PressureFieldComplexSerial &operator-=(const std::complex<double> &rhs);

    PressureFieldComplexSerial &operator*=(const std::complex<double> &rhs);

    PressureFieldComplexSerial &operator/=(const std::complex<double> &rhs);

    std::complex<double> InnerProduct(const PressureFieldComplexSerial &rhs) const;

    std::complex<double> DotProduct(const PressureFieldComplexSerial &rhs) const;

    std::complex<double> DotProduct(const PressureFieldSerial &rhs) const;

    PressureFieldSerial GetRealPart() const;
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

inline PressureFieldComplexSerial operator*(const PressureFieldComplexSerial &t1, const std::complex<double> &t2)
{
    PressureFieldComplexSerial t3(t1);
    t3 *= t2;
    return t3;
}

inline PressureFieldComplexSerial operator*(const std::complex<double> &t1, const PressureFieldComplexSerial &t2) { return t2 * t1; }

inline PressureFieldComplexSerial operator/(const PressureFieldComplexSerial &t1, const double &t2)
{
    PressureFieldComplexSerial t3(t1);
    t3 /= t2;
    return t3;
}
