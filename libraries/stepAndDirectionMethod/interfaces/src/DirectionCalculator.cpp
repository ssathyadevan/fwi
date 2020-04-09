#include "DirectionCalculator.h"

DirectionCalculator::DirectionCalculator(const grid2D &grid, double errorFunctionalScalingfactor, forwardModelInterface *forwardmodel) :
    _grid(grid), _errorFunctionalScalingFactor(errorFunctionalScalingfactor), _forwardmodel(forwardmodel)
{
}

DirectionCalculator::~DirectionCalculator() {}
