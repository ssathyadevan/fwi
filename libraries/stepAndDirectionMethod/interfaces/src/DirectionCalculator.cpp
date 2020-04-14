#include "DirectionCalculator.h"

DirectionCalculator::DirectionCalculator(double errorFunctionalScalingfactor, forwardModelInterface *forwardmodel) :
    _errorFunctionalScalingFactor(errorFunctionalScalingfactor), _forwardmodel()
{
    _forwardmodel = forwardmodel;
    int nFreq = _forwardmodel->getFreq().nFreq;
    std::cout << nFreq << std::endl;
}

DirectionCalculator::~DirectionCalculator() {}
