#pragma once

#include "DirectionCalculator.h"

namespace fwi
{
    namespace inversionMethods
    {
        class ConjugateGradientDirectionCalculator : public DirectionCalculator
        {
        public:
            ConjugateGradientDirectionCalculator(double errorFunctionalScalingFactor, forwardModels::ForwardModelInterface *forwardModel);
			
            virtual ~ConjugateGradientDirectionCalculator();

            /**
             * @brief calculateDirection for conjugate gradient method
             * @param chi is a core::dataGrid2D
             * @param residual is a vector of complex doubles (one value for each combination of core::Sources, core::Receivers and frequencies)
             * @return a core::dataGrid2D which represents the direction towards which to move
             */
            core::dataGrid2D &calculateDirection(const core::dataGrid2D &chi, const std::vector<std::complex<double>> &residual) override;

        private:
            /**
             * @brief calculateGammaPolakRibiere computes the value Gamma as defined by eq. (38) in /doc/ReadMe/1_ProjectDescription.pdf.
             * This method does not get invoked in the first iteration of the inversion.
             * @return the value Gamma used in eq. (35) of /doc/ReadMe/1_ProjectDescription.pdf
             */
            double calculateGammaPolakRibiere();

            core::dataGrid2D _directionPrevious;
            core::dataGrid2D _zeta;
        };
    }   // namespace inversionMethods
}   // namespace fwi
