#include "VariableStepSizeCalculator.h"

VariableStepSizeCalculator::VariableStepSizeCalculator(double start, double slope) : _start(start), _slope(slope) {}

VariableStepSizeCalculator::~VariableStepSizeCalculator() {}

double VariableStepSizeCalculator::calculateStepSize()
{
    double test = 5.0;
    return test;
}

void VariableStepSizeCalculator::updateVariables(const dataGrid2D &, const dataGrid2D &, int iteration) { _stepIterator = iteration; }
