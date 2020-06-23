#pragma once

#include "DirectionCalculator.h"

namespace inversionMethods
{
    class ConjugateGradientDirectionCalculator : public DirectionCalculator
    {
    public:
        ConjugateGradientDirectionCalculator(double errorFunctionalScalingFactor, forwardModels::forwardModelInterface *forwardmodel);
        virtual ~ConjugateGradientDirectionCalculator();

        /**
         * @brief calculateDirection for conjugate gradient method
         * @param chi is a core::dataGrid2D
         * @param residual is a vector of complex doubles (one value for each combination of core::sources, core::receivers and frequencies)
         * @return a core::dataGrid2D which represents the direction
         */
        core::dataGrid2D &calculateDirection(const core::dataGrid2D &chi, const std::vector<std::complex<double>> &residual) override;
    };
}   // namespace inversionMethods
