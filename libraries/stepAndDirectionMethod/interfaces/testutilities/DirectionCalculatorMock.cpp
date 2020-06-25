#include "DirectionCalculatorMock.h"

namespace fwi
{
    namespace inversionMethods
    {
        DirectionCalculatorMock::DirectionCalculatorMock(double errorFunctionalScalingfactor, forwardModels::forwardModelInterface *forwardModel)
            : DirectionCalculator(errorFunctionalScalingfactor, forwardModel)
        {
        }

        DirectionCalculatorMock::~DirectionCalculatorMock() {}

        core::dataGrid2D &DirectionCalculatorMock::calculateDirection(const core::dataGrid2D &, const std::vector<std::complex<double>> &)
        {
            return _direction;
        }
    }   // namespace inversionMethods
}   // namespace fwi
