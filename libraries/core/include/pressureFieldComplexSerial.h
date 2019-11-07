#pragma once

#include "pressureFieldComplex.h"
#include "grid2D.h"
#include "pressureFieldSerial.h"

#include <iostream>
#include <complex>
#include <cassert>
#include <functional>
#include <array>
#include <vector>
#include <string>


class PressureFieldComplexSerial : public PressureFieldComplex
{
private:
    std::complex<double> * const data;

public:
    PressureFieldComplexSerial(const Grid2D &grid)
        : PressureFieldComplex(grid), data(new std::complex<double>[this->GetNumberOfGridPoints()])
    {}

    PressureFieldComplexSerial(const PressureFieldComplexSerial &rhs);

    virtual ~PressureFieldComplexSerial();

    virtual void SetField(const std::function< std::complex<double>(double,double) > func);

    virtual void toBuffer(std::complex<double> *buffer) const;

    virtual void fromBuffer(const std::complex<double> *buffer);

    virtual void toFile(const std::string &fileName) const;

    virtual void fromFile(const std::string &fileName);

    virtual void Zero();

    virtual void Square();

    virtual void Reciprocal();

    virtual PressureFieldComplexSerial& Conjugate();

    virtual void Random();

    virtual double Norm() const;

    virtual double RelNorm() const;

    const std::complex<double>* GetDataPtr() const;

    std::complex<double>* GetDataPtr();

    PressureFieldComplexSerial& operator=(const PressureFieldComplexSerial& rhs);

    PressureFieldComplexSerial& operator=(const PressureFieldSerial& rhs);

    PressureFieldComplexSerial& operator=(const double& rhs);

    PressureFieldComplexSerial& operator+=(const PressureFieldComplexSerial& rhs);

    PressureFieldComplexSerial& operator-=(const PressureFieldComplexSerial& rhs);

    PressureFieldComplexSerial& operator*=(const PressureFieldComplexSerial& rhs);

    PressureFieldComplexSerial& operator*=(const PressureFieldSerial& rhs);

    PressureFieldComplexSerial & operator+=(const double& rhs);

    PressureFieldComplexSerial& operator-=(const double& rhs);

    PressureFieldComplexSerial& operator*=(const double& rhs);

    PressureFieldComplexSerial& operator/=(const double& rhs);

    PressureFieldComplexSerial & operator+=(const std::complex<double>& rhs);

    PressureFieldComplexSerial & operator-=(const std::complex<double>& rhs);

    PressureFieldComplexSerial & operator*=(const std::complex<double>& rhs);

    PressureFieldComplexSerial & operator/=(const std::complex<double>& rhs);

    std::complex<double> InnerProduct(const PressureFieldComplexSerial& rhs) const;

    std::complex<double> Summation(const PressureFieldComplexSerial& rhs) const;

    std::complex<double> Summation(const PressureFieldSerial& rhs) const;

    PressureFieldSerial GetRealPart() const;
};


inline std::complex<double> InnerProduct(const PressureFieldComplexSerial &t1, const PressureFieldComplexSerial &t2)
{
    return t1.InnerProduct(t2);
}

inline std::complex<double> Summation(const PressureFieldComplexSerial &t1, const PressureFieldComplexSerial &t2)
{
    return t1.Summation(t2);
}

inline std::complex<double> Summation(const PressureFieldComplexSerial &t1, const PressureFieldSerial &t2)
{
    return t1.Summation(t2);
}

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

inline PressureFieldComplexSerial operator*(const PressureFieldSerial &t1, const PressureFieldComplexSerial &t2)
{
    return t2*t1;
}

inline PressureFieldComplexSerial operator*(const PressureFieldComplexSerial &t1, const std::complex<double> &t2)
{
    PressureFieldComplexSerial t3(t1);
    t3 *= t2;
    return t3;
}

inline PressureFieldComplexSerial operator*(const std::complex<double> &t1, const PressureFieldComplexSerial &t2)
{
    return t2*t1;
}

inline PressureFieldComplexSerial operator/(const PressureFieldComplexSerial &t1, const double &t2)
{
    PressureFieldComplexSerial t3(t1);
    t3 /= t2;
    return t3;
}



