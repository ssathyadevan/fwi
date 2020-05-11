#pragma once
#include "dataGrid2D.h"

struct StepSizeParameters
{
public:
    double initialStepSize;
    double slope;

    StepSizeParameters() : initialStepSize(0.0), slope(0.0) {}
    StepSizeParameters(double initialStepSize_, double slope_) : initialStepSize(initialStepSize_), slope(slope_) {}
};
