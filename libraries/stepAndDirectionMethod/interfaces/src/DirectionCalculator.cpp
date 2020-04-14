#include "DirectionCalculator.h"

DirectionCalculator::DirectionCalculator(double errorFunctionalScalingfactor, forwardModelInterface *forwardmodel) :
    _errorFunctionalScalingFactor(errorFunctionalScalingfactor), _forwardmodel()
{
    if(errorFunctionalScalingfactor < 0.0)
    {
        throw std::invalid_argument("Error functional scaling factor is negative");
    }
    _forwardmodel = forwardmodel;
}

DirectionCalculator::~DirectionCalculator() {}
