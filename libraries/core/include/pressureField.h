#pragma once

#include <functional>
#include <genericInput.h>
#include <grid2D.h>
#include <iostream>
#include <string>

class pressureField
{
protected:
    const grid2D &_grid;

    pressureField(const pressureField &) = delete;
    pressureField &operator=(const pressureField &) = delete;

public:
    pressureField(const grid2D &grid_) : _grid(grid_) {}
    virtual ~pressureField() {}

    const grid2D &GetGrid() const { return _grid; }
    int getNumberOfGridPoints() const { return _grid.getNumberOfGridPoints(); }
    double getCellVolume() const { return _grid.getCellVolume(); }

    virtual void zero() = 0;
    virtual void square() = 0;
    virtual void sqrt() = 0;
    virtual void reciprocal() = 0;

    virtual double norm() const = 0;
    virtual double relNorm() const = 0;
    virtual double summation() const = 0;

    virtual void random() = 0;
    virtual void randomSaurabh() = 0;

    virtual void toBuffer(double *buffer) const = 0;
    virtual void fromBuffer(const double *buffer) = 0;

    virtual void toFile(const std::string &fileName) const = 0;
    virtual void fromFile(const genericInput &input) = 0;
};
