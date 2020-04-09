#pragma once

class StepSizeInterface
{
public:
    StepSizeInterface();
    ~StepSizeInterface();

protected:
    virtual double calculateStepSize() = 0;
};
