#pragma once

#include "deltaAmplification.h"
#include "iter1.h"

struct ConjugateGradientInversionInput
{
    iter1 iteration1;
    DeltaAmplification dAmplification;
    bool doRegularisation;
    int n_max;
};
