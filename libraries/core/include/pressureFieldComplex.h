#pragma once

#include <complex>
#include <functional>
#include <grid2D.h>
#include <iostream>
#include <pressureField.h>
#include <string>

class pressureFieldComplex
{
private:
    const grid2D &_grid;

    pressureFieldComplex(const pressureFieldComplex &) = delete;
    pressureFieldComplex &operator=(const pressureFieldComplex &) = delete;

public:
    pressureFieldComplex(const grid2D &grid_);
    virtual ~pressureFieldComplex() {}

    const grid2D &GetGrid() const { return _grid; }
    int getNumberOfGridPoints() const { return _grid.getNumberOfGridPoints(); }
    double getCellVolume() const { return _grid.getCellVolume(); }

    virtual void zero() = 0;
    virtual void square() = 0;
    virtual void sqrt() = 0;
    virtual void reciprocal() = 0;

    virtual double norm() const = 0;
    virtual double relNorm() const = 0;
    virtual std::complex<double> Summation() const = 0;

    virtual void toBuffer(std::complex<double> *buffer) const = 0;
    virtual void fromBuffer(const std::complex<double> *buffer) = 0;

    virtual void toFile(const std::string &fileName) const = 0;
    virtual void fromFile(const std::string &fileName) = 0;
};
