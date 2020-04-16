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
    void setCurrentVariables(const dataGrid2D &, const dataGrid2D &);
    void updateVariables(const dataGrid2D &, const dataGrid2D &);

private:
    double _size = 0;
};
