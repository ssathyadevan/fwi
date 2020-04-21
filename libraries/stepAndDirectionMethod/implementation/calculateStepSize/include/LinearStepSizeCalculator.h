#pragma once
#include "StepSizeCalculator.h"
#include "grid2D.h"

class LinearStepSizeCalculator : public StepSizeCalculator
{
public:
    LinearStepSizeCalculator(double start, double slope);
    virtual ~LinearStepSizeCalculator();

    double calculateStepSize();
    void updateVariables(const dataGrid2D &, const dataGrid2D &, int iteration);

private:
    double _start;
    double _slope;
    int _iteration;
};
