#include "ConjugateGradientDirectionCalculator.h"

ConjugateGradientDirectionCalculator::ConjugateGradientDirectionCalculator(const grid2D &grid, double eta) : DirectionCalculator(grid, eta) {}

ConjugateGradientDirectionCalculator::~ConjugateGradientDirectionCalculator() {}

void ConjugateGradientDirectionCalculator::calculateDirection(const complexDataGrid2D &residual)
{
    (void)residual;
    _direction = 1.0;
}
