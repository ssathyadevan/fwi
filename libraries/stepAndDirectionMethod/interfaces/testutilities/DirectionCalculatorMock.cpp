#include "DirectionCalculatorMock.h"

DirectionCalculatorMock::DirectionCalculatorMock(const grid2D &grid, double eta) : DirectionCalculator(grid, eta) {}

DirectionCalculatorMock::~DirectionCalculatorMock() {}

dataGrid2D DirectionCalculatorMock::calculateDirection(const dataGrid2D &, const complexDataGrid2D &) { return dataGrid2D(_grid); }
