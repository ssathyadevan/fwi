#pragma once

#include "DirectionCalculator.h"

namespace fwi
{
    namespace inversionMethods
    {
        class DummyDirectionCalculator : public DirectionCalculator
        {
        public:
            DummyDirectionCalculator(double errorFunctionalScalingFactor, forwardModels::forwardModelInterface *forwardModel);
            virtual ~DummyDirectionCalculator();

            /**
             * @brief calculateDirection computes the direction towards which to move
             * @param chi is a core::dataGrid2D
             * @param residual is a vector of complex doubles (one value for each combination of core::Sources, core::Receivers and frequencies)
             * @return a core::dataGrid2D which represents the direction
             */
            core::dataGrid2D &calculateDirection(const core::dataGrid2D &chi, const std::vector<std::complex<double>> &residual) override;
        };
    }   // namespace inversionMethods
}   // namespace fwi
