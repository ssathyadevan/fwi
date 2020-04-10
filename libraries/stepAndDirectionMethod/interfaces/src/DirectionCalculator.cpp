#include "DirectionCalculator.h"

DirectionCalculator::DirectionCalculator(double errorFunctionalScalingfactor, forwardModelInterface *forwardmodel) :
    _errorFunctionalScalingFactor(errorFunctionalScalingfactor), _forwardmodel(forwardmodel)
{
}

DirectionCalculator::~DirectionCalculator() {}
