#pragma once

#include <cassert>
#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>
#include "freq.h"

class FrequenciesGroup
{

    FrequenciesGroup operator=(const FrequenciesGroup&) = delete;

public:

  const int nFreq;
  const double c_0;
  const double d_freq;
  std::vector<double> freq;
  std::vector<double> k;

  FrequenciesGroup(Freq freqStruct, double c_0_);

  void Print(int nFreqTotal);
};
