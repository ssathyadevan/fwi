#pragma once

#include "DirectionCalculator.h"

namespace fwi
{
    namespace inversionMethods
    {
        class DirectionCalculatorMock : public DirectionCalculator
        {
        public:
            DirectionCalculatorMock(double errorFunctionalScalingfactor, forwardModels::ForwardModelInterface *forwardModel);
            virtual ~DirectionCalculatorMock();
            core::dataGrid2D &calculateDirection(const core::dataGrid2D &, const std::vector<std::complex<double>> &) override;
        };
    }   // namespace inversionMethods
}   // namespace fwi
