#ifndef FREQUENCIES_GROUP_H
#define FREQUENCIES_GROUP_H


#include <cassert>
#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>
#include "frequenciesGroup.h"
#include "freq.h"

class frequenciesGroup
{

    frequenciesGroup operator=(const frequenciesGroup&);

public:

  const int nFreq;
  const double c_0, &d_freq;
  std::vector<double> freq;
  std::vector<double> k;

  frequenciesGroup(Freq freqStruct, double c_0_);

  void Print(int nFreqTotal);
};
#endif /* FREQUENCIES_GROUP_H */
