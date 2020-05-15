#pragma once

#include "StepSizeCalculator.h"

class FixedStepSizeCalculator : public StepSizeCalculator
{
public:
    FixedStepSizeCalculator(double size);
    virtual ~FixedStepSizeCalculator();

    double calculateStepSize();
    void updateVariables(const dataGrid2D &, const dataGrid2D &, int);

private:
    double _size = 0;
};
