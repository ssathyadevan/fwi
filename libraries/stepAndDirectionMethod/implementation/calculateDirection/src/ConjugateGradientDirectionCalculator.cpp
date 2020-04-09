#include "ConjugateGradientDirectionCalculator.h"

ConjugateGradientDirectionCalculator::ConjugateGradientDirectionCalculator(
    const grid2D &grid, double errorFunctionalScalingFactor, forwardModelInterface *forwardmodel) :
    DirectionCalculator(grid, errorFunctionalScalingFactor, forwardmodel)
{
}

ConjugateGradientDirectionCalculator::~ConjugateGradientDirectionCalculator() {}

dataGrid2D ConjugateGradientDirectionCalculator::calculateDirection(const dataGrid2D &, const complexDataGrid2D &residual)
{
    (void)residual;
    dataGrid2D direction(_grid);
    direction = 1.0;
    return direction;
}
