#ifndef VOLFIELD_RECT_2D_CPU_H
#define VOLFIELD_RECT_2D_CPU_H
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
#include "genericInput.h"

class pressureFieldSerial : public pressureField
{

    double * const data;

public:

    explicit pressureFieldSerial(const grid2D &grid);

    pressureFieldSerial(const pressureFieldSerial &rhs);

    virtual ~pressureFieldSerial();

    // Virtual overrides
    virtual void Zero();

    virtual void Random();
    virtual void RandomSaurabh(); //Generates random approximations of Saurabh

    virtual void toBuffer(double *buffer) const;

    virtual void fromBuffer(const double *buffer);

    virtual void toFile(const std::string &fileName) const;

    virtual void fromFile(const genericInput &input);

    virtual void SetField(const std::function< double(double,double) > func);

    virtual double Norm() const;
    virtual double RelNorm() const;
    virtual void Square();
    virtual void Sqrt();
    virtual void Reciprocal();
    virtual double Summation() const;

    // Non virtual members
    double InnerProduct(const pressureFieldSerial &y) const;
    double Summation(const pressureFieldSerial &rhs) const;
    void Gradient(pressureFieldSerial **output);

    pressureFieldSerial& operator=(const pressureFieldSerial& rhs);
    pressureFieldSerial& operator=(const double rhs);
    pressureFieldSerial& operator-=(const pressureFieldSerial &rhs);
    pressureFieldSerial& operator*=(const pressureFieldSerial &rhs);
    pressureFieldSerial& operator/=(const pressureFieldSerial &rhs);
    pressureFieldSerial& operator-=(const double rhs);
    pressureFieldSerial& operator*=(const double rhs);
    pressureFieldSerial& operator/=(const double rhs);

    const double *GetDataPtr() const { return data; }
    double *GetDataPtr() { return data; }

    void CopyTo(pressureFieldSerial &dest);


    pressureFieldSerial& operator+=(const pressureFieldSerial &rhs);

    pressureFieldSerial& operator+=(const double rhs);

};

inline double InnerProduct(const pressureFieldSerial &x, const pressureFieldSerial &y)
{
    return x.InnerProduct(y);
}

inline pressureFieldSerial operator+(const pressureFieldSerial &x, const pressureFieldSerial &y)
{
    pressureFieldSerial result(x);
    result += y;
    return result;
}

inline pressureFieldSerial operator+(const pressureFieldSerial& x, const double y)
{
    pressureFieldSerial result(x);
    result += y;
    return result;
}

inline pressureFieldSerial operator-(const pressureFieldSerial &x, const pressureFieldSerial &y)
{
    pressureFieldSerial result(x);
    result -= y;
    return result;
}

inline pressureFieldSerial operator*(const pressureFieldSerial &x, const pressureFieldSerial &y)
{
    pressureFieldSerial result(x);
    result *= y;
    return result;
}

inline pressureFieldSerial operator*(const double x, const pressureFieldSerial &y)
{
    pressureFieldSerial result(y);
    result *= x;
    return result;
}

inline pressureFieldSerial operator/(const pressureFieldSerial &x, const pressureFieldSerial &y)
{
    pressureFieldSerial result(x);
    result /= y;
    return result;
}

#endif /* VOLFIELD_RECT_2D_CPU_H */


