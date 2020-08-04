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

            // getUpdateDirectionInformation() updates kappaTimesResidual with the new residual vector
            _forwardModel->getUpdateDirectionInformation(residual, kappaTimesResidual);

            // here we add g_n as in eq. (35) of /doc/ReadMe/1_ProjectDescription.pdf
            core::dataGrid2D minusGradient = 2.0 * _errorFunctionalScalingFactor * kappaTimesResidual.getRealPart();
            _direction = minusGradient;

            // this ensures that in the first step of the inversion process we do not add the gamma*zeta term,
            // and also that we never risk dividing by 0 in calculateGammaPolakRibiere().
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

            // gammaNumerator corresponds to int (g_n(x)*(g_n(x) -g_{n-1}(x)) dx
            gammaNumerator = _direction.innerProduct(_direction - _directionPrevious);
            // gammaDenominator corresponds to int (g_{n-1}(x)^2)dx = ||g_{n-1}||^2
            gammaDenominator = _directionPrevious.innerProduct(_directionPrevious);

            return gammaNumerator / gammaDenominator;
        }

    }   // namespace inversionMethods
}   // namespace fwi
