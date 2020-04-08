#pragma once

#include "DirectionCalculator.h"

class ConjugateGradientDirectionCalculator : public DirectionCalculator
{
public:
    ConjugateGradientDirectionCalculator(const grid2D &grid, double eta);
    virtual ~ConjugateGradientDirectionCalculator();

    virtual void calculateDirection(const complexDataGrid2D &residual);
};
