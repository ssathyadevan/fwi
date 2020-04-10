#include "GradientDescentDirectionCalculator.h"

GradientDescentDirectionCalculator::GradientDescentDirectionCalculator(
    double errorFunctionalScalingFactor, forwardModelInterface *forwardmodel, const dataGrid2D &pdata) :
    DirectionCalculator(errorFunctionalScalingFactor, forwardmodel),
    _pData(pdata)
{
}

GradientDescentDirectionCalculator::~GradientDescentDirectionCalculator() {}

dataGrid2D GradientDescentDirectionCalculator::calculateDirection(const dataGrid2D &chiEsitmate, const complexDataGrid2D &)
{
    dataGrid2D direction(chiEsitmate.getGrid());
    direction = 1.0;

    return direction;
}
