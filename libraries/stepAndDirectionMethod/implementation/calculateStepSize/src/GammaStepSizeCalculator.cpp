#include "GammaStepSizeCalculator.h"

GammaStepSizeCalculator::GammaStepSizeCalculator(const dataGrid2D currentChi, const vector<double> currentDerivative) :
    _currentChi(currentChi), _currentDerivative(currentDerivative)
{
}

GammaStepSizeCalculator::~GammaStepSizeCalculator() {}

double GammaStepSizeCalculator::calculateStepSize()
{
    double test = 5.2;
    return test;
}
