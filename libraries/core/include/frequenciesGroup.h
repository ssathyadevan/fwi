#ifndef FREQUENCIES_GROUP_H
#define FREQUENCIES_GROUP_H

#include "variable_structure.h"

#include <cassert>
#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>

class Frequencies_group
{

    Frequencies_group operator=(const Frequencies_group&);

public:

  const int nFreq;
  const double c_0, &d_freq;
  std::vector<double> freq;
  std::vector<double> k;

  Frequencies_group(Freq freqStruct, double c_0_);

  void Print(int nFreqTotal);
};
#endif /* FREQUENCIES_GROUP_H */
