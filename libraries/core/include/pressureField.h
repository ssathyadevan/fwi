#pragma once

#include <functional>
#include <genericInput.h>
#include <grid2D.h>
#include <iostream>
#include <string>

class PressureField
{
protected:
    const Grid2D &_grid;

    PressureField(const PressureField &) = delete;
    PressureField &operator=(const PressureField &) = delete;

public:
    PressureField(const Grid2D &grid_) : _grid(grid_) {}
    virtual ~PressureField() {}

    const Grid2D &GetGrid() const { return _grid; }
    int GetNumberOfGridPoints() const { return _grid.GetNumberOfGridPoints(); }
    double getCellVolume() const { return _grid.GetCellVolume(); }

    virtual void Zero() = 0;
    virtual void Square() = 0;
    virtual void Sqrt() = 0;
    virtual void Reciprocal() = 0;

    virtual double Norm() const = 0;
    virtual double RelNorm() const = 0;
    virtual double Summation() const = 0;

    virtual void Random() = 0;
    virtual void RandomSaurabh() = 0;

    virtual void toBuffer(double *buffer) const = 0;
    virtual void fromBuffer(const double *buffer) = 0;

    virtual void toFile(const std::string &fileName) const = 0;
    virtual void fromFile(const GenericInput &input) = 0;
};
