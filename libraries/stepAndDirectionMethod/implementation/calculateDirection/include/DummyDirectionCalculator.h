#pragma once

#include "DirectionCalculator.h"

namespace fwi
{
    namespace inversionMethods
    {
        /**
         * @brief The DummyDirectionCalculator class is a dummy example to implement a DirectionCalculator for the StepAndDirectionMethod
         */
        class DummyDirectionCalculator : public DirectionCalculator
        {
        public:
            DummyDirectionCalculator(double errorFunctionalScalingFactor, forwardModels::forwardModelInterface *const forwardModel);
            virtual ~DummyDirectionCalculator();

            // Review: I think that the documentation is not correct in this case since
            // calculateDirection always returns zero for this class
            // answer: the explanation is meant for a generic calculateDirection() method, not this one in particular
            /**
             * @brief calculateDirection computes the direction towards which to move
             * @param chi is a core::dataGrid2D
             * @param residual is a vector of complex doubles (one value for each combination of core::Sources, core::Receivers and core::FrequenciesGroup)
             * @return a core::dataGrid2D which represents the direction towards which we want to move
             */
            core::dataGrid2D &calculateDirection(const core::dataGrid2D &chi, const std::vector<std::complex<double>> &residual) override;
        };
    }   // namespace inversionMethods
}   // namespace fwi
