#include "VariableStepSizeCalculator.h"

namespace inversionMethods
{
    VariableStepSizeCalculator::VariableStepSizeCalculator(double start, double slope)
        : _start(start)
        , _slope(slope)
    {
    }

    VariableStepSizeCalculator::~VariableStepSizeCalculator() {}

    double VariableStepSizeCalculator::calculateStepSize()
    {
        double test = 5.0;
        return test;
    }

    void VariableStepSizeCalculator::updateVariables(
        const core::dataGrid2D &, const core::dataGrid2D &, int iteration, const std::vector<std::complex<double>> &, const std::vector<std::complex<double>> &)
    {
        _stepIterator = iteration;
    }
}   // namespace inversionMethods
