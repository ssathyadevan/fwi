#ifndef FREQUENCIES_H
#define FREQUENCIES_H

#include <cassert>
#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>

template <typename T>
class Frequencies {

	Frequencies<T> operator=(const Frequencies<T>&);

public:

  const int nFreq;
  const T c_0;
  std::vector<T> freq;
  std::vector<T> k;

  Frequencies(T freqMin, T freqMax, int nFreq_, T c_0_) : nFreq(nFreq_), c_0(c_0_), freq(), k() {

    assert(nFreq > 1);

    T dfreq = (freqMax - freqMin) / (nFreq - 1);

    for(int i=0; i < nFreq; i++) {
      freq.push_back(freqMin + i * dfreq);
      T omega = 2.0 * M_PI * freq[i];
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

#endif /* FREQUENCIES_H */
