#include "conjugateGradientDirectionCalculator.h"

conjugateGradientDirectionCalculator::conjugateGradientDirectionCalculator(const grid2D &grid) : directionCalculator(grid) {}

conjugateGradientDirectionCalculator::~conjugateGradientDirectionCalculator() {}

void conjugateGradientDirectionCalculator::calculateDirection(const complexDataGrid2D &residual)
{
    (void)residual;
    _direction = 1.0;
}
