#pragma once
#include "dataGrid2D.h"
class StepSizeCalculator
{
public:
    StepSizeCalculator();
    virtual ~StepSizeCalculator();

    virtual double calculateStepSize() = 0;
    virtual void setCurrentVariables(const dataGrid2D &, const dataGrid2D &) = 0;
    virtual void updateVariables(const dataGrid2D &, const dataGrid2D &) = 0;
};
