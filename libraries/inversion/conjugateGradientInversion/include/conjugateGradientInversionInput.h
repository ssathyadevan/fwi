#pragma once

#include "deltaAmplification.h"
#include "iter1.h"

namespace inversionMethods
{
    struct ConjugateGradientInversionInput
    {
        iter1 iteration1;
        cgDeltaAmplification dAmplification;
        bool doRegularisation;
        int n_max;
    };
}   // namespace inversionMethods
