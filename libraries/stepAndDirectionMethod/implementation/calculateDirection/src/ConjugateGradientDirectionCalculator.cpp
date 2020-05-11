#include "ConjugateGradientDirectionCalculator.h"

ConjugateGradientDirectionCalculator::ConjugateGradientDirectionCalculator(double errorFunctionalScalingFactor, forwardModelInterface *forwardmodel) :
    DirectionCalculator(errorFunctionalScalingFactor, forwardmodel)
{
}

ConjugateGradientDirectionCalculator::~ConjugateGradientDirectionCalculator() {}

dataGrid2D &ConjugateGradientDirectionCalculator::calculateDirection(const dataGrid2D &chi, const std::vector<std::complex<double>> &residual)
{
    complexDataGrid2D kappaTimesResidual(chi.getGrid());

    _forwardModel->getUpdateDirectionInformation(residual, kappaTimesResidual);
    _direction = _errorFunctionalScalingFactor * kappaTimesResidual.getRealPart();

    return _direction;
}
