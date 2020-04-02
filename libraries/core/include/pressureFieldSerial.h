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

class pressureFieldSerial : public pressureField
{
private:
    std::vector<double> _data;
    double *const _dataPointer;

public:
    explicit pressureFieldSerial(const grid2D &grid);
    pressureFieldSerial(const pressureFieldSerial &rhs);
    ~pressureFieldSerial() {}

    // Virtual overrides
    void zero() override;
    void square() override;
    void sqrt() override;
    void reciprocal() override;

    double norm() const override { return std::sqrt(innerProduct(*this)); }
    double relNorm() const override { return std::sqrt(innerProduct(*this) / getNumberOfGridPoints()); }
    double summation() const override;

    void random() override;          // unused?
    void randomSaurabh() override;   // Generates random approximations of Saurabh

    void toBuffer(double *buffer) const override;
    void fromBuffer(const double *buffer) override;

    void toFile(const std::string &fileName) const override;
    void fromFile(const genericInput &input) override;

    // Non virtual members
    const std::vector<double> &getData() const { return _data; }

    void setValueAtIndex(const double value, const int index) { _data[index] = value; }
    void addValueAtIndex(const double value, const int index) { _data[index] += value; }

    void randomChild(const pressureFieldSerial &parent, std::default_random_engine &generator, std::normal_distribution<double> &distribution);

    double innerProduct(const pressureFieldSerial &rhs) const;
    void gradient(std::vector<pressureFieldSerial> &gradientField) const;

    void CopyTo(pressureFieldSerial &dest) { dest = *this; }

    // TODO: REMOVE getDataPtr()
    const double *getDataPtr() const { return _dataPointer; }
    double *getDataPtr() { return _dataPointer; }

    // Operators
    void PlusElement(const int location, const double value);
    pressureFieldSerial &operator=(const pressureFieldSerial &rhs);
    pressureFieldSerial &operator=(const std::vector<double> &rhs);
    pressureFieldSerial &operator=(const double rhs);

    pressureFieldSerial &operator+=(const pressureFieldSerial &rhs);
    pressureFieldSerial &operator+=(const std::vector<double> &rhs);
    pressureFieldSerial &operator+=(const double rhs);

    pressureFieldSerial &operator-=(const pressureFieldSerial &rhs);
    pressureFieldSerial &operator-=(const std::vector<double> &rhs);
    pressureFieldSerial &operator-=(const double rhs);

    pressureFieldSerial &operator*=(const pressureFieldSerial &rhs);
    pressureFieldSerial &operator*=(const std::vector<double> &rhs);
    pressureFieldSerial &operator*=(const double rhs);

    pressureFieldSerial &operator/=(const pressureFieldSerial &rhs);
    pressureFieldSerial &operator/=(const std::vector<double> &rhs);
    pressureFieldSerial &operator/=(const double rhs);
};

inline double innerProduct(const pressureFieldSerial &x, const pressureFieldSerial &y) { return x.innerProduct(y); }

inline pressureFieldSerial operator+(const pressureFieldSerial &x, const pressureFieldSerial &y)
{
    pressureFieldSerial result(x);
    result += y;
    return result;
}

inline pressureFieldSerial operator+(const pressureFieldSerial &x, const double y)
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
