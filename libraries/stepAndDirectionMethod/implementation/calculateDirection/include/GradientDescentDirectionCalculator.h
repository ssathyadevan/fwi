#pragma once

#include "DirectionCalculator.h"

class GradientDescentDirectionCalculator : public DirectionCalculator
{
public:
    GradientDescentDirectionCalculator(const grid2D &grid, double errorFunctionalScalingFactor, const dataGrid2D &pData);
    virtual ~GradientDescentDirectionCalculator();

    dataGrid2D calculateDirection(const dataGrid2D &, const complexDataGrid2D &) override;

private:
    dataGrid2D _pData;
};
