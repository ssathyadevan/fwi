#ifndef FREQUENCIES_GROUP_H
#define FREQUENCIES_GROUP_H

#include <cassert>
#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>
#include "variable_structure.h"

/*
   Babak 2018 11 05: Detemplating the class
*/
//use this one for group frequency distribution

class Frequencies_group {

    Frequencies_group operator=(const Frequencies_group&);

public:

  const int nFreq;
  const double c_0, &d_freq;
  std::vector<double> freq;
  std::vector<double> k;

  Frequencies_group(Freq freqStruct, double c_0_)//double freqMin, const double &dfreq, int nFreq_, double c_0_)
  : nFreq(freqStruct.nTotal), c_0(c_0_), d_freq(freqStruct.spacing), freq(), k()
  {

    assert(nFreq > 1);

    for(int i=0; i < freqStruct.nTotal; i++) {
      freq.push_back(freqStruct.min + i * freqStruct.spacing);
      double omega = 2.0 * M_PI * freq[i];
      k.push_back(omega / c_0);
    }

  }

  void Print(int nFreqTotal) {
    std::cout << "Total number of frequencies is " << nFreqTotal << ". Values:" << std::endl;
    for(int i=0; i<nFreqTotal; i++) {
      std::cout << "f = " << freq[i] << ", k = " << k[i] << std::endl;
    }
    std::cout << std::endl;
  }

};

#endif /* FREQUENCIES_GROUP_H */
