#include "ConjugateGradientDirectionCalculator.h"

ConjugateGradientDirectionCalculator::ConjugateGradientDirectionCalculator(double errorFunctionalScalingFactor, forwardModelInterface *forwardmodel) :
    DirectionCalculator(errorFunctionalScalingFactor, forwardmodel)
{
}

ConjugateGradientDirectionCalculator::~ConjugateGradientDirectionCalculator() {}

dataGrid2D ConjugateGradientDirectionCalculator::calculateDirection(const dataGrid2D &chi, const std::vector<std::complex<double>> &residual)
{
    dataGrid2D direction(chi.getGrid());
    complexDataGrid2D kappaTimesResidual(chi.getGrid());

    _forwardmodel->getUpdateDirectionInformation(residual, kappaTimesResidual);
    direction = _errorFunctionalScalingFactor * kappaTimesResidual.getRealPart();

    return direction;
}
