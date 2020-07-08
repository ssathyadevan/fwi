#include "DummyDirectionCalculator.h"

namespace fwi
{
    namespace inversionMethods
    {
        DummyDirectionCalculator::DummyDirectionCalculator(double errorFunctionalScalingFactor, forwardModels::forwardModelInterface *forwardModel)
            : DirectionCalculator(errorFunctionalScalingFactor, forwardModel)

        {
            _direction.zero();
        }

        DummyDirectionCalculator::~DummyDirectionCalculator() {}

        core::dataGrid2D &DummyDirectionCalculator::calculateDirection(const core::dataGrid2D &chi, const std::vector<std::complex<double>> &residual)
        {
            (void)chi;
            (void)residual;

            return _direction;
        }

    }   // namespace inversionMethods
}   // namespace fwi
