#pragma once

class StepSizeCalculator
{
public:
    StepSizeCalculator();
    ~StepSizeCalculator();

protected:
    virtual double calculateStepSize() = 0;
};
