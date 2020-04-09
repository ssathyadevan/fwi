#pragma once

#include "DirectionCalculator.h"

class DirectionCalculatorMock : public DirectionCalculator
{
public:
    DirectionCalculatorMock(const grid2D &grid, double errorFunctionalScalingfactor, forwardModelInterface *forwardmodel);
    virtual ~DirectionCalculatorMock();
    dataGrid2D calculateDirection(const dataGrid2D &, const complexDataGrid2D &) override;
};
