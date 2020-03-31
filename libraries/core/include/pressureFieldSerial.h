#pragma once

#include "genericInput.h"
#include "grid2D.h"
#include "pressureField.h"
#include <array>
#include <cassert>
#include <cmath>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <random>
#include <string.h>

class PressureFieldSerial : public PressureField
{
private:
    std::vector<double> _data;
    double *const _dataPointer;

public:
    explicit PressureFieldSerial(const Grid2D &grid);

    PressureFieldSerial(const PressureFieldSerial &rhs);

    // Virtual overrides
    void Zero() override;
    void Square() override;
    void Sqrt() override;
    double Norm() const override { return std::sqrt(InnerProduct(*this)); }
    double RelNorm() const override { return std::sqrt(InnerProduct(*this) / GetNumberOfGridPoints()); }
    double Summation() const override;

    void Random() override;          // unused?
    void RandomSaurabh() override;   // Generates random approximations of Saurabh

    void toBuffer(double *buffer) const override;
    void fromBuffer(const double *buffer) override;

    void toFile(const std::string &fileName) const override;

    void SetField(const std::function<double(double, double)> func) override;

    // Non virtual members
    const std::vector<double> &getData() const { return _data; }

    void setData(const std::vector<double> data);
    void setValueAtIndex(const double value, const int index) { _data[index] = value; }

    void addData(const std::vector<double> data);
    void addValueAtIndex(const double value, const int index) { _data[index] += value; }

    void Reciprocal();
    void RandomChild(const PressureFieldSerial &parent, std::default_random_engine &generator, std::normal_distribution<double> &distribution);
    void fromFile(const GenericInput &input);

    double InnerProduct(const PressureFieldSerial &rhs) const;
    void Gradient(std::vector<PressureFieldSerial> &gradientField) const;

    void CopyTo(PressureFieldSerial &dest) { dest = *this; }

    // TODO: REMOVE GetDataPtr()
    const double *GetDataPtr() const { return _dataPointer; }
    double *GetDataPtr() { return _dataPointer; }

    // Operators
    PressureFieldSerial &operator=(const PressureFieldSerial &rhs);
    PressureFieldSerial &operator=(const double rhs);
    PressureFieldSerial &operator+=(const PressureFieldSerial &rhs);
    PressureFieldSerial &operator+=(const double rhs);
    PressureFieldSerial &operator-=(const PressureFieldSerial &rhs);
    PressureFieldSerial &operator-=(const double rhs);
    PressureFieldSerial &operator*=(const PressureFieldSerial &rhs);
    PressureFieldSerial &operator*=(const double rhs);
    PressureFieldSerial &operator/=(const PressureFieldSerial &rhs);
    PressureFieldSerial &operator/=(const double rhs);
};

inline double innerProduct(const PressureFieldSerial &x, const PressureFieldSerial &y) { return x.InnerProduct(y); }

inline PressureFieldSerial operator+(const PressureFieldSerial &x, const PressureFieldSerial &y)
{
    PressureFieldSerial result(x);
    result += y;
    return result;
}

inline PressureFieldSerial operator+(const PressureFieldSerial &x, const double y)
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
