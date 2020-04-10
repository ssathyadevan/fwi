#include "VariableStepSizeCalculator.h"

VariableStepSizeCalculator::VariableStepSizeCalculator(double start, double slope) : _start(start), _slope(slope) {}

VariableStepSizeCalculator::~VariableStepSizeCalculator() {}

double VariableStepSizeCalculator::calculateStepSize()
{
    double test = 5.0;
    return test;
}
