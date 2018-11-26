#ifndef COLCOMPLEXFIELD_RECT_2D_CPU_H
#define COLCOMPLEXFIELD_RECT_2D_CPU_H

#include <iostream>
#include <complex>
#include <cassert>
#include <functional>
#include <array>
#include <vector>
#include <string>

#include "volComplexField_rect_2D.h"
#include "grid_rect_2D.h"
#include "volField_rect_2D_cpu.h"

class volComplexField_rect_2D_cpu : public volComplexField_rect_2D
{
private:
    std::complex<double> * const data;

public:
    volComplexField_rect_2D_cpu(const grid_rect_2D &grid)
        : volComplexField_rect_2D(grid), data(new std::complex<double>[this->GetNumberOfGridPoints()])
    {}

    volComplexField_rect_2D_cpu(const volComplexField_rect_2D_cpu &rhs);

    virtual ~volComplexField_rect_2D_cpu();

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

    volComplexField_rect_2D_cpu& operator=(const volComplexField_rect_2D_cpu& rhs);

    volComplexField_rect_2D_cpu& operator=(const volField_rect_2D_cpu& rhs);

    volComplexField_rect_2D_cpu& operator=(const double& rhs);

    volComplexField_rect_2D_cpu& operator+=(const volComplexField_rect_2D_cpu& rhs);

    volComplexField_rect_2D_cpu& operator-=(const volComplexField_rect_2D_cpu& rhs);

    volComplexField_rect_2D_cpu& operator*=(const volComplexField_rect_2D_cpu& rhs);

    volComplexField_rect_2D_cpu& operator*=(const volField_rect_2D_cpu& rhs);

    volComplexField_rect_2D_cpu & operator+=(const double& rhs);

    volComplexField_rect_2D_cpu& operator-=(const double& rhs);

    volComplexField_rect_2D_cpu& operator*=(const double& rhs);

    volComplexField_rect_2D_cpu& operator/=(const double& rhs);

    volComplexField_rect_2D_cpu & operator+=(const std::complex<double>& rhs);

    volComplexField_rect_2D_cpu & operator-=(const std::complex<double>& rhs);

    volComplexField_rect_2D_cpu & operator*=(const std::complex<double>& rhs);

    volComplexField_rect_2D_cpu & operator/=(const std::complex<double>& rhs);

    std::complex<double> InnerProduct(const volComplexField_rect_2D_cpu& rhs) const;

    std::complex<double> Summation(const volComplexField_rect_2D_cpu& rhs) const;

    std::complex<double> Summation(const volField_rect_2D_cpu& rhs) const;

    volField_rect_2D_cpu GetRealPart() const;
};


inline std::complex<double> InnerProduct(const volComplexField_rect_2D_cpu &t1, const volComplexField_rect_2D_cpu &t2)
{
    return t1.InnerProduct(t2);
}



inline std::complex<double> Summation(const volComplexField_rect_2D_cpu &t1, const volComplexField_rect_2D_cpu &t2)
{
    return t1.Summation(t2);
}


/*template<typename T>
std::complex<double> Summation(const volComplexField_rect_2D_cpu<T> &t1, const volField_rect_2D_cpu<T> &t2, const int &conju)
{
    std::complex<double> sum;
    if (conju==0) ////if conju==0 then do not take the conjugate of Kappa
        sum = t1.Summation(t2);
    else if(conju == 1) ////if conju==1 then take the conjugate of Kappa
        sum = t1.Summation(t2,conju);

    return sum;
}*/



inline std::complex<double> Summation(const volComplexField_rect_2D_cpu &t1, const volField_rect_2D_cpu &t2)
{
    return t1.Summation(t2);
}



inline volComplexField_rect_2D_cpu operator-(const volComplexField_rect_2D_cpu &t1, const volComplexField_rect_2D_cpu &t2)
{
    volComplexField_rect_2D_cpu t3(t1);
    t3 -= t2;
    return t3;
}


inline volComplexField_rect_2D_cpu operator*(const volComplexField_rect_2D_cpu &t1, const volField_rect_2D_cpu &t2)
{
    volComplexField_rect_2D_cpu t3(t1);
    t3 *= t2;
    return t3;
}



inline volComplexField_rect_2D_cpu operator*(const volComplexField_rect_2D_cpu &t1, const volComplexField_rect_2D_cpu &t2)
{
    volComplexField_rect_2D_cpu t3(t1);
    t3 *= t2;
    return t3;
}


inline volComplexField_rect_2D_cpu operator*(const volField_rect_2D_cpu &t1, const volComplexField_rect_2D_cpu &t2)
{
    return t2*t1;
}


inline volComplexField_rect_2D_cpu operator*(const volComplexField_rect_2D_cpu &t1, const std::complex<double> &t2)
{
    volComplexField_rect_2D_cpu t3(t1);
    t3 *= t2;
    return t3;
}



inline volComplexField_rect_2D_cpu operator*(const std::complex<double> &t1, const volComplexField_rect_2D_cpu &t2)
{
    return t2*t1;
}


inline volComplexField_rect_2D_cpu operator/(const volComplexField_rect_2D_cpu &t1, const double &t2)
{
    volComplexField_rect_2D_cpu t3(t1);
    t3 /= t2;
    return t3;
}
#endif // COLCOMPLEXFIELD_RECT_2D_CPU_H



