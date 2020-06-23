#pragma once
#include "DeltaAmplification.h"

namespace fwi
{
    namespace inversionMethods
    {
        struct ConjugateGradientWithRegularisationParametersInput
        {
            DeltaAmplification _deltaAmplification;
            int _nIterations;
            double _tolerance;
        };
    }   // namespace inversionMethods
}   // namespace fwi
