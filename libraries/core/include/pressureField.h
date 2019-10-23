#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <grid2D.h>


class pressureField
{

protected:

    const grid2D &grid;
    const int nGridPoints;
    const double cellVolume;

    pressureField(const pressureField&) = delete;
    pressureField& operator=(const pressureField&) = delete;

public:

    pressureField(const grid2D &grid_) :
        grid(grid_),
        nGridPoints(grid.GetNumberOfGridPoints()),
        cellVolume(grid.GetCellVolume()) {}

    virtual ~pressureField() {}

    const grid2D &GetGrid() const { return grid; }

    int GetNumberOfGridPoints() const { return nGridPoints; }
    double getCellVolume() const { return cellVolume; }

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

