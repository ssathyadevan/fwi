#pragma once

#include "iter1.h"
#include "deltaAmplification.h"

struct conjugateGradientInversionInput
{
      iter1 iteration1;
      deltaAmplification dAmplification;
      bool doReg;
      int n_max;
};
