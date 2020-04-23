#include "DirectionCalculator.h"

DirectionCalculator::DirectionCalculator(double errorFunctionalScalingfactor, forwardModelInterface *forwardModel) :
    _errorFunctionalScalingFactor(errorFunctionalScalingfactor), _forwardModel()
{
    if(errorFunctionalScalingfactor < 0.0)
    {
        throw std::invalid_argument("Error functional scaling factor is negative");
    }
    _forwardModel = forwardModel;
}

DirectionCalculator::~DirectionCalculator() {}
