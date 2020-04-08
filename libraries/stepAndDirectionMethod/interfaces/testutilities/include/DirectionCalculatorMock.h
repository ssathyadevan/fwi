#pragma once

#include "DirectionCalculator.h"

class DirectionCalculatorMock : public DirectionCalculator
{
public:
    DirectionCalculatorMock(const grid2D &grid, double eta);
    virtual ~DirectionCalculatorMock();
};
