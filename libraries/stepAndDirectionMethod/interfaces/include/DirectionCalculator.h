#pragma once

#include "complexDataGrid2D.h"
#include "dataGrid2D.h"
#include "forwardModelInterface.h"

namespace fwi
{
    namespace inversionMethods
    {
        class DirectionCalculator
        {
        public:
            DirectionCalculator(double errorFunctionalScalingFactor, forwardModels::forwardModelInterface *forwardModel);
            virtual ~DirectionCalculator();

            /**
             * @brief calculateDirection
             * @param chi is a core::dataGrid2D, pData is a vector of complex doubles (one value for each combination of core::sources, core::receivers and
             * frequencies)
             * @return a datacore::grid2D which represents the direction
             */
            virtual core::dataGrid2D &calculateDirection(const core::dataGrid2D &, const std::vector<std::complex<double>> &) = 0;

            double getErrorFunctionalScalingFactor() const { return _errorFunctionalScalingFactor; }

        protected:
            const double _errorFunctionalScalingFactor;   // = eta
            forwardModels::forwardModelInterface *_forwardModel;
            core::dataGrid2D _direction;
        };
    }   // namespace inversionMethods
}   // namespace fwi
