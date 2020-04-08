#pragma once

#include "directionCalculator.h"

class gradientDescentDirectionCalculator : public directionCalculator
{
public:
    gradientDescentDirectionCalculator(const grid2D &grid);
    virtual ~gradientDescentDirectionCalculator();

    virtual void calculateDirection(const dataGrid2D &pData, const dataGrid2D &chiEsitmate);
};
