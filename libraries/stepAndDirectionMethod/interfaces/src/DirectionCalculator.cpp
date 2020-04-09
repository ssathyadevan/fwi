#include "DirectionCalculator.h"

DirectionCalculator::DirectionCalculator(const grid2D &grid, double errorFunctionalScalingfactor) :
    _grid(grid), _errorFunctionalScalingFactor(errorFunctionalScalingfactor)
{
}

DirectionCalculator::~DirectionCalculator() {}
