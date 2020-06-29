#pragma once

#include "StepSizeCalculator.h"

class DummyStepSizeCalculator : public StepSizeCalculator
{
public:
    DummyStepSizeCalculator();
    DummyStepSizeCalculator(double dummyStepSize);
    virtual ~DummyStepSizeCalculator();

    double calculateStepSize();
    void updateVariables(const dataGrid2D &chiEstimateCurrent, const dataGrid2D &derivativeCurrent, int iteration,
        const std::vector<std::complex<double>> &kappaTimesDirection, const std::vector<std::complex<double>> &);

private:
    double _dummyStepSize;
};
