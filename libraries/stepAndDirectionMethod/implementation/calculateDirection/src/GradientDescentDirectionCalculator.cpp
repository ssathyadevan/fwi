#include "GradientDescentDirectionCalculator.h"

GradientDescentDirectionCalculator::GradientDescentDirectionCalculator(const grid2D &grid, double eta) : DirectionCalculator(grid, eta) {}

void GradientDescentDirectionCalculator::calculateDirection(const dataGrid2D &pData, const dataGrid2D &chiEsitmate)
{
    (void)pData;
    (void)chiEsitmate;
    _direction = 1.0;
}
