#include "GradientDescentDirectionCalculator.h"

GradientDescentDirectionCalculator::GradientDescentDirectionCalculator(const grid2D &grid, double errorFunctionalScalingFactor, const dataGrid2D &pdata) :
    DirectionCalculator(grid, errorFunctionalScalingFactor), _pData(pdata)
{
}

GradientDescentDirectionCalculator::~GradientDescentDirectionCalculator() {}

dataGrid2D GradientDescentDirectionCalculator::calculateDirection(const dataGrid2D &chiEsitmate, const complexDataGrid2D &)
{
    (void)chiEsitmate;
    dataGrid2D direction(_grid);
    direction = 1.0;

    return direction;
}
