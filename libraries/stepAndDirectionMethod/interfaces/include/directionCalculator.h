#pragma once

#include "complexDataGrid2D.h"
#include "dataGrid2D.h"

class directionCalculator
{
public:
    directionCalculator(const grid2D &grid);
    virtual ~directionCalculator();

    virtual void calculateDirection(const complexDataGrid2D &) { throw std::bad_exception(); };
    virtual void calculateDirection(const dataGrid2D &, const dataGrid2D &) { throw std::bad_exception(); };
    const dataGrid2D &getDirection() { return _direction; }
    const grid2D &getGrid() { return _grid; }

protected:
    grid2D _grid;
    dataGrid2D _direction;
};
