#pragma once

#include <complex>
#include <functional>
#include <grid2D.h>
#include <iostream>
#include <pressureField.h>
#include <string>

class PressureFieldComplex
{
private:
    const Grid2D &_grid;

    PressureFieldComplex(const PressureFieldComplex &) = delete;
    PressureFieldComplex &operator=(const PressureFieldComplex &) = delete;

public:
    PressureFieldComplex(const Grid2D &grid_);
    virtual ~PressureFieldComplex() {}

    const Grid2D &GetGrid() const { return _grid; }
    int GetNumberOfGridPoints() const { return _grid.GetNumberOfGridPoints(); }
    double getCellVolume() const { return _grid.GetCellVolume(); }

    virtual void Zero() = 0;
    virtual void Square() = 0;
    virtual void Sqrt() = 0;
    virtual void Reciprocal() = 0;

    virtual double Norm() const = 0;
    virtual double RelNorm() const = 0;
    virtual std::complex<double> Summation() const = 0;

    virtual void toBuffer(std::complex<double> *buffer) const = 0;
    virtual void fromBuffer(const std::complex<double> *buffer) = 0;

    virtual void toFile(const std::string &fileName) const = 0;
    virtual void fromFile(const std::string &fileName) = 0;
};
