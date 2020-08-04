#include "ConjugateGradientDirectionCalculator.h"

namespace fwi
{
    namespace inversionMethods
    {
        ConjugateGradientDirectionCalculator::ConjugateGradientDirectionCalculator(
            double errorFunctionalScalingFactor, forwardModels::ForwardModelInterface *forwardModel)
            : DirectionCalculator(errorFunctionalScalingFactor, forwardModel)
        {
        }

        ConjugateGradientDirectionCalculator::~ConjugateGradientDirectionCalculator() {}

        core::dataGrid2D &ConjugateGradientDirectionCalculator::calculateDirection(
            const core::dataGrid2D &chi, const std::vector<std::complex<double>> &residual)
        {
            core::complexDataGrid2D kappaTimesResidual(chi.getGrid());

            _forwardModel->getUpdateDirectionInformation(residual, kappaTimesResidual);
            _direction = _errorFunctionalScalingFactor * kappaTimesResidual.getRealPart();

            return _direction;
        }
    }   // namespace inversionMethods
}   // namespace fwi
