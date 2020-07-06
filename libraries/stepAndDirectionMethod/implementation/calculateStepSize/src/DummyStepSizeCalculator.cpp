#include "DummyStepSizeCalculator.h"

DummyStepSizeCalculator::DummyStepSizeCalculator()
    : _dummyStepSize(0.0)
{
}

DummyStepSizeCalculator::DummyStepSizeCalculator(double dummyStepSize)
    : _dummyStepSize(dummyStepSize)
{
}

DummyStepSizeCalculator::~DummyStepSizeCalculator() {}

double DummyStepSizeCalculator::calculateStepSize() { return _dummyStepSize; }

void DummyStepSizeCalculator::updateVariables(const dataGrid2D &chiEstimateCurrent, const dataGrid2D &derivativeCurrent, int iteration,
    const std::vector<std::complex<double>> &kappaTimesDirection, const std::vector<std::complex<double>> &residualVector)
{
    // These are the variables passed by StepAndDirectionReconstructor::reconstruct.
    // The ones that are not used should have their variable names removed in this file and in the header in the method inputs.
    (void)chiEstimateCurrent;
    (void)derivativeCurrent;
    (void)iteration;
    (void)kappaTimesDirection;
    (void)residualVector;
}
