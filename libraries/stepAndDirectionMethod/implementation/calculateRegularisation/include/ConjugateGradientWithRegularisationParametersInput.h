#pragma once
#include "DeltaAmplification.h"

struct ConjugateGradientWithRegularisationParametersInput
{
    DeltaAmplification _deltaAmplification;
    int _nIterations;
    double _tolerance;
};
