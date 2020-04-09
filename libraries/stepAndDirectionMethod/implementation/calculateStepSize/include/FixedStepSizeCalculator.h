#pragma once

#include "stepSizeInterface.h"
class FixedSizeCalculator : public StepSizeInterface
{
public:
    FixedSizeCalculator(_size);
    ~FixedSizeCalculator();

protected:
    double calculateStepSize() override;
};
