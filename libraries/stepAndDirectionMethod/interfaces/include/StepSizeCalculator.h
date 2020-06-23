#pragma once
#include "dataGrid2D.h"
#include <complex>

namespace inversionMethods
{
    class StepSizeCalculator
    {
    public:
        StepSizeCalculator();
        virtual ~StepSizeCalculator();

        virtual double calculateStepSize() = 0;
        virtual void updateVariables(
            const core::dataGrid2D &, const core::dataGrid2D &, int, const std::vector<std::complex<double>> &, const std::vector<std::complex<double>> &) = 0;
    };
}   // namespace inversionMethods
