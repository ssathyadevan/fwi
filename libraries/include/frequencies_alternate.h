#ifndef FREQUENCIES_ALTERNATE_H
#define FREQUENCIES_ALTERNATE_H

#include <cassert>
#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>

/*
   Saurabh 2018 11 13: Detemplating the class
*/
//use this one for alternate frequency distribution
class Frequencies_alternate {

    Frequencies_alternate operator=(const Frequencies_alternate&);

public:

  const int nFreq;
  const double c_0, &d_freq;
  std::vector<double> freq;
  std::vector<double> k;

  Frequencies_alternate(double freqMin, const double &dfreq, int nFreq_, double c_0_)
  : nFreq(nFreq_), c_0(c_0_), d_freq(dfreq), freq(), k()
  {

    assert(nFreq > 1);

    for(int i=0; i < nFreq; i++) {
      freq.push_back(freqMin + i * dfreq);
      double omega = 2.0 * M_PI * freq[i];
      k.push_back(omega / c_0);
    }

  }

  void Print() {
    std::cout << "Total number of frequencies is " << nFreq << ". Values:" << std::endl;
    for(int i=0; i<nFreq; i++) {
      std::cout << "f = " << freq[i] << ", k = " << k[i] << std::endl;
    }
    std::cout << std::endl;
  }

};

#endif // FREQUENCY_ALTERNATE_H
