#include "DirectionCalculatorMock.h"

DirectionCalculatorMock::DirectionCalculatorMock(double errorFunctionalScalingfactor, forwardModelInterface *forwardModel) :
    DirectionCalculator(errorFunctionalScalingfactor, forwardModel)
{
}

DirectionCalculatorMock::~DirectionCalculatorMock() {}

core::dataGrid2D &DirectionCalculatorMock::calculateDirection(const core::dataGrid2D &, const std::vector<std::complex<double>> &) { return _direction; }
