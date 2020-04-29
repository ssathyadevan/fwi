#pragma once

struct DirectionParameters
{
public:
    double derivativeStepSize;

    DirectionParameters() : derivativeStepSize(0.0) {}
    DirectionParameters(double derivativeStepSize_) : derivativeStepSize(derivativeStepSize_) {}
};
