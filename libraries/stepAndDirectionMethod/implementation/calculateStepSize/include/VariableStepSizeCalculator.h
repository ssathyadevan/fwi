#pragma once

#include "StepSizeCalculator.h"

namespace inversionMethods
{
    class VariableStepSizeCalculator : public StepSizeCalculator
    {
    public:
        VariableStepSizeCalculator(double start, double slope);
        virtual ~VariableStepSizeCalculator();

        double calculateStepSize();
        void updateVariables(const core::dataGrid2D &, const core::dataGrid2D &, int iteration, const std::vector<std::complex<double>> &,
            const std::vector<std::complex<double>> &);

    private:
        int _stepIterator = 0;   // If this calculator is used multiple times, reset this iterator.
        const double _start;
        const double _slope;
    };
}   // namespace inversionMethods
