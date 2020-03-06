#pragma once

#include "pressureField.h"
#include "grid2D.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <array>
#include <functional>
#include <cassert>
#include <string.h>
#include <random>
#include "genericInput.h"

class PressureFieldSerial : public PressureField
{

    double * const data; // initialised on heap by constructor: data(new double[grid_size]).

public:

    explicit PressureFieldSerial(const Grid2D &grid);

    PressureFieldSerial(const PressureFieldSerial &rhs);

    virtual ~PressureFieldSerial();

    // Virtual overrides
    virtual void Zero();

    virtual void Random(); // unused?
    virtual void RandomSaurabh(); //Generates random approximations of Saurabh
    virtual void RandomChild(const PressureFieldSerial &parent, std::default_random_engine &generator, std::normal_distribution<double> &distribution);

    virtual void toBuffer(double *buffer) const;

    virtual void fromBuffer(const double *buffer);

    virtual void toFile(const std::string &fileName) const;

    virtual void fromFile(const GenericInput &input);

    virtual void SetField(const std::function< double(double,double) > func);

    virtual double Norm() const;
    virtual double RelNorm() const;
    virtual void Square();
    virtual void Sqrt();
    virtual void Reciprocal();
    virtual double Summation() const;

    // Non virtual members
    double InnerProduct(const PressureFieldSerial &rhs) const;
    void Gradient(PressureFieldSerial **output);

    void PlusElement ( const int location, const double value);

    PressureFieldSerial& operator=(const PressureFieldSerial& rhs);
    PressureFieldSerial& operator=(const double rhs);
    PressureFieldSerial& operator-=(const PressureFieldSerial &rhs);
    PressureFieldSerial& operator*=(const PressureFieldSerial &rhs);
    PressureFieldSerial& operator/=(const PressureFieldSerial &rhs);
    PressureFieldSerial& operator-=(const double rhs);
    PressureFieldSerial& operator*=(const double rhs);
    PressureFieldSerial& operator/=(const double rhs);

    const double *GetDataPtr() const { return data; }
    double *GetDataPtr() { return data; }

    void CopyTo(PressureFieldSerial &dest);


    PressureFieldSerial& operator+=(const PressureFieldSerial &rhs);

    PressureFieldSerial& operator+=(const double rhs);
};

inline double InnerProduct(const PressureFieldSerial &x, const PressureFieldSerial &y)
{
    return x.InnerProduct(y);
}

inline PressureFieldSerial operator+(const PressureFieldSerial &x, const PressureFieldSerial &y)
{
    PressureFieldSerial result(x);
    result += y;
    return result;
}

inline PressureFieldSerial operator+(const PressureFieldSerial& x, const double y)
{
    PressureFieldSerial result(x);
    result += y;
    return result;
}

inline PressureFieldSerial operator-(const PressureFieldSerial &x, const PressureFieldSerial &y)
{
    PressureFieldSerial result(x);
    result -= y;
    return result;
}

inline PressureFieldSerial operator*(const PressureFieldSerial &x, const PressureFieldSerial &y)
{
    PressureFieldSerial result(x);
    result *= y;
    return result;
}

inline PressureFieldSerial operator*(const double x, const PressureFieldSerial &y)
{
    PressureFieldSerial result(y);
    result *= x;
    return result;
}

inline PressureFieldSerial operator/(const PressureFieldSerial &x, const PressureFieldSerial &y)
{
    PressureFieldSerial result(x);
    result /= y;
    return result;
}


