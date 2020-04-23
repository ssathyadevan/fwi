#pragma once
#include "DeltaAmplification.h"

struct ConjugateGradientWithRegularisationParametersInput
{
    DeltaAmplification _deltaAmplification;
    int _nRegularisationIterations;
    double _tolerance;
};
