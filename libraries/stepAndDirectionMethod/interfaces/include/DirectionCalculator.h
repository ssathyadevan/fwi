#pragma once

#include "complexDataGrid2D.h"
#include "dataGrid2D.h"
#include "forwardModelInterface.h"

class DirectionCalculator
{
public:
    DirectionCalculator(double errorFunctionalScalingFactor, forwardModelInterface *forwardModel);
    virtual ~DirectionCalculator();

    /**
     * @brief calculateDirection
     * @param chi is a dataGrid2D, pData is a vector of complex doubles (one value for each combination of sources, receivers and frequencies)
     * @return a datagrid2D which represents the direction
     */
    virtual dataGrid2D calculateDirection(const dataGrid2D &, const std::vector<std::complex<double>> &) = 0;

    double getErrorFunctionalScalingFactor() const { return _errorFunctionalScalingFactor; }

protected:
    const double _errorFunctionalScalingFactor;
    forwardModelInterface *_forwardmodel;
};
