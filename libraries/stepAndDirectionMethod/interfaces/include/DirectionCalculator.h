#pragma once

#include "complexDataGrid2D.h"
#include "dataGrid2D.h"

class DirectionCalculator
{
public:
    DirectionCalculator(const grid2D &grid, const double eta);
    virtual ~DirectionCalculator();

    virtual void calculateDirection(const complexDataGrid2D &) { throw std::bad_exception(); };
    virtual void calculateDirection(const dataGrid2D &, const dataGrid2D &) { throw std::bad_exception(); };
    const dataGrid2D &getDirection() { return _direction; }
    const grid2D &getGrid() { return _grid; }

protected:
    grid2D _grid;
    double _eta;
    dataGrid2D _direction;
};
