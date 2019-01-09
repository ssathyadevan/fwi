#ifndef VOLFIELD_RECT_2D_CPU_H
#define VOLFIELD_RECT_2D_CPU_H
#include "volField_rect_2D.h"
#include "variable_structure.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <array>
#include <functional>
#include <cassert>
#include <string.h>


class volField_rect_2D_cpu : public volField_rect_2D
{

    double * const data;

public:

    volField_rect_2D_cpu(const grid_rect_2D &grid);

    volField_rect_2D_cpu(const volField_rect_2D_cpu &rhs);

    virtual ~volField_rect_2D_cpu();

    // Virtual overrides
    virtual void Zero();

    virtual void Random();
    virtual void RandomSaurabh();


    virtual void toBuffer(double *buffer) const;

    virtual void fromBuffer(const double *buffer);

    virtual void toFile(const std::string &fileName) const;

    virtual void fromFile(const Input &input);

    virtual void SetField(const std::function< double(double,double) > func);

    virtual double Norm() const;
    virtual double RelNorm() const;
    virtual void Square();
    virtual void Sqrt();
    virtual void Reciprocal();
    virtual double Summation() const;

    // Non virtual members
    double InnerProduct(const volField_rect_2D_cpu &y) const;
    double Summation(const volField_rect_2D_cpu &rhs) const;
    void Gradient(volField_rect_2D_cpu **output);

    volField_rect_2D_cpu& operator=(const volField_rect_2D_cpu& rhs);
    volField_rect_2D_cpu& operator=(const double rhs);
    volField_rect_2D_cpu& operator-=(const volField_rect_2D_cpu &rhs);
    volField_rect_2D_cpu& operator*=(const volField_rect_2D_cpu &rhs);
    volField_rect_2D_cpu& operator/=(const volField_rect_2D_cpu &rhs);
    volField_rect_2D_cpu& operator-=(const double rhs);
    volField_rect_2D_cpu& operator*=(const double rhs);
    volField_rect_2D_cpu& operator/=(const double rhs);

    const double *GetDataPtr() const { return data; }
    double *GetDataPtr() { return data; }

    void CopyTo(volField_rect_2D_cpu &dest);


    volField_rect_2D_cpu& operator+=(const volField_rect_2D_cpu &rhs);

    volField_rect_2D_cpu& operator+=(const double rhs);

};

inline double InnerProduct(const volField_rect_2D_cpu &x, const volField_rect_2D_cpu &y)
{
    return x.InnerProduct(y);
}

inline volField_rect_2D_cpu operator+(const volField_rect_2D_cpu &x, const volField_rect_2D_cpu &y)
{
    volField_rect_2D_cpu result(x);
    result += y;
    return result;
}

inline volField_rect_2D_cpu operator+(const volField_rect_2D_cpu& x, const double y)
{
    volField_rect_2D_cpu result(x);
    result += y;
    return result;
}

inline volField_rect_2D_cpu operator-(const volField_rect_2D_cpu &x, const volField_rect_2D_cpu &y)
{
    volField_rect_2D_cpu result(x);
    result -= y;
    return result;
}

inline volField_rect_2D_cpu operator*(const volField_rect_2D_cpu &x, const volField_rect_2D_cpu &y)
{
    volField_rect_2D_cpu result(x);
    result *= y;
    return result;
}

inline volField_rect_2D_cpu operator*(const double x, const volField_rect_2D_cpu &y)
{
    volField_rect_2D_cpu result(y);
    result *= x;
    return result;
}

inline volField_rect_2D_cpu operator/(const volField_rect_2D_cpu &x, const volField_rect_2D_cpu &y)
{
    volField_rect_2D_cpu result(x);
    result /= y;
    return result;
}

#endif /* VOLFIELD_RECT_2D_CPU_H */


