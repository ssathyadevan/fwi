#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <grid2D.h>


class PressureField
{

protected:

    const Grid2D &grid;
    const int nGridPoints; // These are the same properties as contained in Grid2D. Why repeat?
    const double cellVolume; // These are the same properties as contained in Grid2D. Why repeat?

    PressureField(const PressureField&) = delete;
    PressureField& operator=(const PressureField&) = delete;

public:

    PressureField(const Grid2D &grid_) :
        grid(grid_),
        nGridPoints(grid.GetNumberOfGridPoints()),
        cellVolume(grid.GetCellVolume()) {}

    virtual ~PressureField() {}

    const Grid2D &GetGrid() const { return grid; }

    int GetNumberOfGridPoints() const { return nGridPoints; } // These are the same properties as contained in Grid2D. Why repeat?
    double getCellVolume() const { return cellVolume; } // These are the same properties as contained in Grid2D. Why repeat?

    virtual void Zero() = 0;
    virtual void Square() = 0;
    virtual void Sqrt() = 0;
    virtual void Random() = 0;
    virtual void RandomSaurabh() = 0;


    virtual void toBuffer(double *buffer) const = 0;
    virtual void fromBuffer(const double *buffer) = 0;

    virtual void toFile(const std::string &fileName) const = 0;

    virtual void SetField(const std::function< double(double,double) > func) = 0;
    virtual double Norm() const = 0;
    virtual double RelNorm() const = 0;
    virtual double Summation() const = 0;


};

