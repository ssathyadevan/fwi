#pragma once

#include "DirectionCalculator.h"

class ConjugateGradientDirectionCalculator : public DirectionCalculator
{
public:
    ConjugateGradientDirectionCalculator(double errorFunctionalScalingFactor, forwardModelInterface *forwardmodel);
    virtual ~ConjugateGradientDirectionCalculator();

    /**
     * @brief calculateDirection for conjugate gradient method
     * @param chi is a dataGrid2D
     * @param residual is a vector of complex doubles (one value for each combination of sources, receivers and frequencies)
     * @return a dataGrid2D which represents the direction
     */
    dataGrid2D &calculateDirection(const dataGrid2D &chi, const std::vector<std::complex<double>> &residual) override;
};
