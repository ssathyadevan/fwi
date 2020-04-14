#include "ConjugateGradientDirectionCalculator.h"

ConjugateGradientDirectionCalculator::ConjugateGradientDirectionCalculator(double errorFunctionalScalingFactor, forwardModelInterface *forwardmodel) :
    DirectionCalculator(errorFunctionalScalingFactor, forwardmodel)
{
}

ConjugateGradientDirectionCalculator::~ConjugateGradientDirectionCalculator() {}

dataGrid2D ConjugateGradientDirectionCalculator::calculateDirection(const dataGrid2D &chi, const std::vector<std::complex<double>> &residual)
{
    dataGrid2D direction(chi.getGrid());
    direction = 1.0;
    return direction;
}
