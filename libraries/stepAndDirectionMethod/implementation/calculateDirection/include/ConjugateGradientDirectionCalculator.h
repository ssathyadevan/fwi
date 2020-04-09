#pragma once

#include "DirectionCalculator.h"

class ConjugateGradientDirectionCalculator : public DirectionCalculator
{
public:
    ConjugateGradientDirectionCalculator(const grid2D &grid, double errorFunctionalScalingFactor);
    virtual ~ConjugateGradientDirectionCalculator();

    dataGrid2D calculateDirection(const dataGrid2D &, const complexDataGrid2D &) override;
};
