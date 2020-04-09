#include "DirectionCalculatorMock.h"

DirectionCalculatorMock::DirectionCalculatorMock(const grid2D &grid, double errorFunctionalScalingfactor, forwardModelInterface *forwardmodel) :
    DirectionCalculator(grid, errorFunctionalScalingfactor, forwardmodel)
{
}

DirectionCalculatorMock::~DirectionCalculatorMock() {}

dataGrid2D DirectionCalculatorMock::calculateDirection(const dataGrid2D &, const complexDataGrid2D &) { return dataGrid2D(_grid); }
