#pragma once

#include "complexDataGrid2D.h"
#include "dataGrid2D.h"
#include "forwardModelInterface.h"

class DirectionCalculator
{
public:
    DirectionCalculator(const grid2D &grid, const double errorFunctionalScalingFactor, forwardModelInterface *forwardModel);
    virtual ~DirectionCalculator();

    virtual dataGrid2D calculateDirection(const dataGrid2D &, const complexDataGrid2D &) = 0;

    const grid2D &getGrid() { return _grid; }
    double getErrorFunctionalScalingFactor() { return _errorFunctionalScalingFactor; }

protected:
    grid2D _grid;
    double _errorFunctionalScalingFactor;
    forwardModelInterface *_forwardmodel;
};
