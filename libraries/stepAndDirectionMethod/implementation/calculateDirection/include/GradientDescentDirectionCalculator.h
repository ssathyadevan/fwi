#pragma once

#include "DirectionCalculator.h"

class GradientDescentDirectionCalculator : public DirectionCalculator
{
public:
    GradientDescentDirectionCalculator(double errorFunctionalScalingFactor, forwardModelInterface *forwardmodel, const dataGrid2D &pData);
    virtual ~GradientDescentDirectionCalculator();

    dataGrid2D calculateDirection(const dataGrid2D &, const complexDataGrid2D &) override;

private:
    dataGrid2D _pData;
};
