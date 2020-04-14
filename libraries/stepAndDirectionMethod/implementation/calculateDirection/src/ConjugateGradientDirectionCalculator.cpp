#include "ConjugateGradientDirectionCalculator.h"

ConjugateGradientDirectionCalculator::ConjugateGradientDirectionCalculator(double errorFunctionalScalingFactor, forwardModelInterface *forwardmodel) :
    DirectionCalculator(errorFunctionalScalingFactor, forwardmodel)
{
}

ConjugateGradientDirectionCalculator::~ConjugateGradientDirectionCalculator() {}

dataGrid2D ConjugateGradientDirectionCalculator::calculateDirection(const dataGrid2D &, const complexDataGrid2D &residual)
{
    dataGrid2D direction(residual.getGrid());
    direction = 1.0;
    return direction;
}
