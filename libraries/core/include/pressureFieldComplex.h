#pragma once

#include <complex>
#include <functional>
#include <grid2D.h>
#include <iostream>
#include <pressureField.h>
#include <string>

class PressureFieldComplex
{
    const Grid2D &_grid;
    const int _nGridPoints;
    const double _cellVolume;
    PressureFieldComplex(const PressureFieldComplex &) = delete;
    PressureFieldComplex &operator=(const PressureFieldComplex &) = delete;

public:
    PressureFieldComplex(const Grid2D &grid_);

    const Grid2D &GetGrid() const { return _grid; }

    int GetNumberOfGridPoints() const { return _nGridPoints; }
    double getCellVolume() const { return _cellVolume; }

    virtual void Zero() = 0;
    virtual void Square() = 0;
    virtual void Reciprocal() = 0;

    virtual double Norm() const = 0;
    virtual double RelNorm() const = 0;
    virtual std::complex<double> Summation() const = 0;

    virtual void toBuffer(std::complex<double> *buffer) const = 0;
    virtual void fromBuffer(const std::complex<double> *buffer) = 0;

    virtual void toFile(const std::string &fileName) const = 0;
    virtual void fromFile(const std::string &fileName) = 0;

    virtual void SetField(const std::function<std::complex<double>(double, double)> func) = 0;
};
