#pragma once
#include "dataGrid2D.h"
#include <complex>
class StepSizeCalculator
{
public:
    StepSizeCalculator();
    virtual ~StepSizeCalculator();

    virtual double calculateStepSize() = 0;
    virtual void updateVariables(
        const dataGrid2D &, const dataGrid2D &, int, const std::vector<std::complex<double>> &, const std::vector<std::complex<double>> &) = 0;
};
