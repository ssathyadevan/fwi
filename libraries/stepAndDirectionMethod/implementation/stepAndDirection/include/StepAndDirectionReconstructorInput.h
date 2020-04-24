#pragma once
#include "DirectionParameters.h"
#include "ReconstructorParameters.h"
#include "StepSizeParameters.h"

struct StepAndDirectionReconstructorInput
{
public:
    ReconstructorParameters reconstructorParameters;
    StepSizeParameters stepSizeParameters;
    DirectionParameters directionParameters;
    bool doConjugateGradientRegularisation;
};
