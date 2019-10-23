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


class pressureFieldComplexSerial : public pressureFieldComplex
{
private:
    std::complex<double> * const data;

public:
    pressureFieldComplexSerial(const grid2D &grid)
        : pressureFieldComplex(grid), data(new std::complex<double>[this->GetNumberOfGridPoints()])
    {}

    pressureFieldComplexSerial(const pressureFieldComplexSerial &rhs);

    virtual ~pressureFieldComplexSerial();

    virtual void SetField(const std::function< std::complex<double>(double,double) > func);

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

    const std::complex<double>* GetDataPtr() const;

    std::complex<double>* GetDataPtr();

    pressureFieldComplexSerial& operator=(const pressureFieldComplexSerial& rhs);

    pressureFieldComplexSerial& operator=(const pressureFieldSerial& rhs);

    pressureFieldComplexSerial& operator=(const double& rhs);

    pressureFieldComplexSerial& operator+=(const pressureFieldComplexSerial& rhs);

    pressureFieldComplexSerial& operator-=(const pressureFieldComplexSerial& rhs);

    pressureFieldComplexSerial& operator*=(const pressureFieldComplexSerial& rhs);

    pressureFieldComplexSerial& operator*=(const pressureFieldSerial& rhs);

    pressureFieldComplexSerial & operator+=(const double& rhs);

    pressureFieldComplexSerial& operator-=(const double& rhs);

    pressureFieldComplexSerial& operator*=(const double& rhs);

    pressureFieldComplexSerial& operator/=(const double& rhs);

    pressureFieldComplexSerial & operator+=(const std::complex<double>& rhs);

    pressureFieldComplexSerial & operator-=(const std::complex<double>& rhs);

    pressureFieldComplexSerial & operator*=(const std::complex<double>& rhs);

    pressureFieldComplexSerial & operator/=(const std::complex<double>& rhs);

    std::complex<double> InnerProduct(const pressureFieldComplexSerial& rhs) const;

    std::complex<double> Summation(const pressureFieldComplexSerial& rhs) const;

    std::complex<double> Summation(const pressureFieldSerial& rhs) const;

    pressureFieldSerial GetRealPart() const;
};


inline std::complex<double> InnerProduct(const pressureFieldComplexSerial &t1, const pressureFieldComplexSerial &t2)
{
    return t1.InnerProduct(t2);
}

inline std::complex<double> Summation(const pressureFieldComplexSerial &t1, const pressureFieldComplexSerial &t2)
{
    return t1.Summation(t2);
}

inline std::complex<double> Summation(const pressureFieldComplexSerial &t1, const pressureFieldSerial &t2)
{
    return t1.Summation(t2);
}

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

inline pressureFieldComplexSerial operator*(const pressureFieldSerial &t1, const pressureFieldComplexSerial &t2)
{
    return t2*t1;
}

inline pressureFieldComplexSerial operator*(const pressureFieldComplexSerial &t1, const std::complex<double> &t2)
{
    pressureFieldComplexSerial t3(t1);
    t3 *= t2;
    return t3;
}

inline pressureFieldComplexSerial operator*(const std::complex<double> &t1, const pressureFieldComplexSerial &t2)
{
    return t2*t1;
}

inline pressureFieldComplexSerial operator/(const pressureFieldComplexSerial &t1, const double &t2)
{
    pressureFieldComplexSerial t3(t1);
    t3 /= t2;
    return t3;
}



