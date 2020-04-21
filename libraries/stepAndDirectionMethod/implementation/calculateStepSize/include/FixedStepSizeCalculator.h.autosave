#pragma once

#include "StepSizeCalculator.h"
#include "grid2D.h"

class FixedStepSizeCalculator : public StepSizeCalculator
{
public:
    FixedStepSizeCalculator(double size);
    virtual ~FixedStepSizeCalculator();

    double calculateStepSize();
    void setStepSize(double size);
    void updateVariables(const dataGrid2D &, const dataGrid2D &, const int &);

private:
    double _size = 0;
};
