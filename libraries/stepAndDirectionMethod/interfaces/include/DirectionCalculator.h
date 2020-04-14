#pragma once

#include "complexDataGrid2D.h"
#include "dataGrid2D.h"
#include "forwardModelInterface.h"

class DirectionCalculator
{
public:
    DirectionCalculator(double errorFunctionalScalingFactor, forwardModelInterface *forwardModel);
    virtual ~DirectionCalculator();

    virtual dataGrid2D calculateDirection(const dataGrid2D &, const complexDataGrid2D &) = 0;

    double getErrorFunctionalScalingFactor() const { return _errorFunctionalScalingFactor; }

protected:
    const double _errorFunctionalScalingFactor;
    forwardModelInterface *_forwardmodel;
};
