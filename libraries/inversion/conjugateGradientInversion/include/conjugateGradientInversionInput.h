#pragma once

#include "iter1.h"
#include "deltaAmplification.h"

struct ConjugateGradientInversionInput
{
      iter1 iteration1;
      deltaAmplification dAmplification;
      bool doRegularisation;
      int n_max;
};
