#pragma once

#include "DirectionCalculator.h"

class GradientDescentDirectionCalculator : public DirectionCalculator
{
public:
    GradientDescentDirectionCalculator(
        double errorFunctionalScalingFactor, forwardModelInterface *forwardmodel, double derivativeStepSize, const std::vector<std::complex<double>> &pData);
    virtual ~GradientDescentDirectionCalculator();

    dataGrid2D calculateDirection(const dataGrid2D &chiEstimate, const complexDataGrid2D &) override;
    double optimizationFunction() const;

private:
    const std::vector<std::complex<double>> &_pData;
    const double _derivativeStepSize;
};
