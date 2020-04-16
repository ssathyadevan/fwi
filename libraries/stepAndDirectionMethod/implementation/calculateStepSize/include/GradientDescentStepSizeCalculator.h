#pragma once

#include "StepSizeCalculator.h"
#include "dataGrid2D.h"

// I want to build only one instance, so either I do a static initialization in the for(nIterations) loop or I build it before empty and fill it at every
// iteration through setVariables (maybe updateVariables is better if I also want to reassign the current values to the old ones)

class GradientDescentStepSizeCalculator : public StepSizeCalculator
{
public:
    GradientDescentStepSizeCalculator(const grid2D chiEstimate);
    virtual ~GradientDescentStepSizeCalculator();

    double calculateStepSize();
    void setCurrentVariables(const dataGrid2D &chiEstimateCurrent, const dataGrid2D &derivativeCurrent);
    void updateVariables(const dataGrid2D &chiEstimateCurrent, const dataGrid2D &derivativeCurrent);

private:
    dataGrid2D _chiEstimatePrevious;
    dataGrid2D _chiEstimateCurrent;

    dataGrid2D _derivativePrevious;
    dataGrid2D _derivativeCurrent;
};
