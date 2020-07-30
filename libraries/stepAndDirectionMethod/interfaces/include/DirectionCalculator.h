#pragma once

#include "ForwardModelInterface.h"
#include "complexDataGrid2D.h"
#include "dataGrid2D.h"

namespace fwi
{
    namespace inversionMethods
    {
        class DirectionCalculator
        {
        public:
            DirectionCalculator(double errorFunctionalScalingFactor, forwardModels::ForwardModelInterface *forwardModel);
            virtual ~DirectionCalculator();

            /**
             * @brief calculateDirection
             * @param chi is a core::dataGrid2D, pData is a vector of complex doubles (one value for each combination of core::Sources, core::Receivers and
             * frequencies)
             * @return a datacore::grid2D which represents the direction
             */
            virtual core::dataGrid2D &calculateDirection(const core::dataGrid2D &, const std::vector<std::complex<double>> &) = 0;

            double getErrorFunctionalScalingFactor() const { return _errorFunctionalScalingFactor; }

        protected:
            const double _errorFunctionalScalingFactor;   // = eta
            forwardModels::ForwardModelInterface *_forwardModel;
            core::dataGrid2D _direction;
        };
    }   // namespace inversionMethods
}   // namespace fwi
