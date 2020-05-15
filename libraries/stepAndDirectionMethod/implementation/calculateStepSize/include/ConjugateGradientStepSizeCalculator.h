#pragma once
#include "StepSizeCalculator.h"

class ConjugateGradientStepSizeCalculator : public StepSizeCalculator
{
private:
    dataGrid2D _chiEstimatePrevious;
    dataGrid2D _chiEstimateCurrent;

    dataGrid2D _derivativePrevious;
    dataGrid2D _derivativeCurrent;

    const double _initialStepSize;
    int _iteration;

public:
    ConjugateGradientStepSizeCalculator(const grid2D &grid, const double initialStepSize);
    virtual ~ConjugateGradientStepSizeCalculator() {}

    double calculateStepSize();
    void updateVariables(const dataGrid2D &chiEstimateCurrent, const dataGrid2D &derivativeCurrent, int iteration);
};
