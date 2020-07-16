#pragma once

#include "DirectionCalculator.h"

namespace fwi
{
    namespace inversionMethods
    {
        // Review: Since we are adding a class it would be nice to document its purpose in a general
        // way like "DummyDirectionCalculator is an inversion method" for doxygen
        class DummyDirectionCalculator : public DirectionCalculator
        {
        public:
		    // Review: errorFunctionalScalingFactor should be const  and forwardModel should be a const ptr so  forwardModels::forwardModelInterface * const
            DummyDirectionCalculator( double errorFunctionalScalingFactor, forwardModels::forwardModelInterface *forwardModel);
            virtual ~DummyDirectionCalculator();

            // Review: I think that the documentation is not correct in this case since
            // calculateDirection always returns zero for this class
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
