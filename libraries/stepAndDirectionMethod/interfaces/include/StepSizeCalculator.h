#pragma once

class StepSizeCalculator
{
public:
    StepSizeCalculator();
    virtual ~StepSizeCalculator();

    virtual double calculateStepSize() = 0;
};
