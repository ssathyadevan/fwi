#include "gradientDescentDirectionCalculator.h"

gradientDescentDirectionCalculator::gradientDescentDirectionCalculator(const grid2D &grid) : directionCalculator(grid) {}

void gradientDescentDirectionCalculator::calculateDirection(const dataGrid2D &pData, const dataGrid2D &chiEsitmate)
{
    (void)pData;
    (void)chiEsitmate;
    _direction = 1.0;
}
