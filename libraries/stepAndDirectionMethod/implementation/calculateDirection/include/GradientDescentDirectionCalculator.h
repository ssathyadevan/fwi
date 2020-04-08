#pragma once

#include "DirectionCalculator.h"

class GradientDescentDirectionCalculator : public DirectionCalculator
{
public:
    GradientDescentDirectionCalculator(const grid2D &grid, double eta);
    virtual ~GradientDescentDirectionCalculator();

    virtual void calculateDirection(const dataGrid2D &pData, const dataGrid2D &chiEsitmate);
};
