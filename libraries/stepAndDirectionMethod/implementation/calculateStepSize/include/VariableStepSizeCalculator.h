#pragma once

#include "StepSizeCalculator.h"

class VariableStepSizeCalculator : public StepSizeCalculator
{
public:
    VariableStepSizeCalculator(double start, double slope);
    virtual ~VariableStepSizeCalculator();

    double calculateStepSize();
    void updateVariables(const dataGrid2D &, const dataGrid2D &, int iteration);

private:
    int _stepIterator = 0;   // If this calculator is used multiple times, reset this iterator.
    const double _start;
    const double _slope;
};
