#pragma once

#include "directionCalculator.h"

class conjugateGradientDirectionCalculator : public directionCalculator
{
public:
    conjugateGradientDirectionCalculator(const grid2D &grid);
    virtual ~conjugateGradientDirectionCalculator();

    virtual void calculateDirection(const complexDataGrid2D &residual);
};
