#include "VariableStepSizeCalculator.h"

VariableStepSizeCalculator::VariableStepSizeCalculator(double start, double slope) : _start(start), _slope(slope) {}

VariableStepSizeCalculator::~VariableStepSizeCalculator() {}

double VariableStepSizeCalculator::calculateStepSize()
{
    double test = 5.0;
    return test;
    //    return start +slope * _stepIterator;
}

void VariableStepSizeCalculator::updateVariables(const dataGrid2D &, const dataGrid2D &, const int &iteration) { _stepIterator = iteration; }
