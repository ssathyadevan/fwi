#include "DirectionCalculatorMock.h"

DirectionCalculatorMock::DirectionCalculatorMock(double errorFunctionalScalingfactor, forwardModelInterface *forwardmodel) :
    DirectionCalculator(errorFunctionalScalingfactor, forwardmodel)
{
}

DirectionCalculatorMock::~DirectionCalculatorMock() {}

dataGrid2D DirectionCalculatorMock::calculateDirection(const dataGrid2D &data, const std::vector<std::complex<double>> &) { return dataGrid2D(data.getGrid()); }
