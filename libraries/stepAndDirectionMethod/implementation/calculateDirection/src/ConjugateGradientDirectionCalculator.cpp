#include "ConjugateGradientDirectionCalculator.h"

namespace fwi
{
    namespace inversionMethods
    {
        ConjugateGradientDirectionCalculator::ConjugateGradientDirectionCalculator(
            double errorFunctionalScalingFactor, forwardModels::ForwardModelInterface *forwardModel)
            : DirectionCalculator(errorFunctionalScalingFactor, forwardModel)
            , _directionPrevious(forwardModel->getGrid())
            , _zeta(forwardModel->getGrid())
        {
        }

        ConjugateGradientDirectionCalculator::~ConjugateGradientDirectionCalculator() {}

        core::dataGrid2D &ConjugateGradientDirectionCalculator::calculateDirection(
            const core::dataGrid2D &chi, const std::vector<std::complex<double>> &residual)
        {
            core::complexDataGrid2D kappaTimesResidual(chi.getGrid());

            _forwardModel->getUpdateDirectionInformation(residual, kappaTimesResidual);

            // here we add g_n as in eq. (35), (37) of /doc/ReadMe/1_ProjectDescription.pdf

            core::dataGrid2D minusGradient = 2.0 * _errorFunctionalScalingFactor * kappaTimesResidual.getRealPart();
            _direction = minusGradient;

            // this ensures that in the first step of the inversion process we do not add this term.
            // It also ensures that the division in calculateGammaPolakRibiere() will not throw exceptions.
            if(_directionPrevious.norm() > 0.0)
            {
                double gamma = calculateGammaPolakRibiere();
                _direction += gamma * _zeta;
            }

            _directionPrevious = minusGradient;
            _zeta = _direction;

            return _direction;
        }

        double ConjugateGradientDirectionCalculator::calculateGammaPolakRibiere()
        {
            double gammaNumerator = 0.0;
            double gammaDenominator = 0.0;

            gammaNumerator = _direction.innerProduct(_direction - _directionPrevious);
            gammaDenominator = _directionPrevious.innerProduct(_directionPrevious);

            return gammaNumerator / gammaDenominator;
        }

    }   // namespace inversionMethods
}   // namespace fwi
