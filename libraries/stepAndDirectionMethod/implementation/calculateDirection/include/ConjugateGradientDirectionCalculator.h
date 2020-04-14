#pragma once

#include "DirectionCalculator.h"

class ConjugateGradientDirectionCalculator : public DirectionCalculator
{
public:
    ConjugateGradientDirectionCalculator(double errorFunctionalScalingFactor, forwardModelInterface *forwardmodel);
    virtual ~ConjugateGradientDirectionCalculator();

    dataGrid2D calculateDirection(const dataGrid2D &chi, const std::vector<std::complex<double>> &residual) override;
};
