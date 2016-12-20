#ifndef HELPER_H
#define HELPER_H

#include <cmath>
#include <iostream>

template <typename T>
inline T dist(T x, T z) {
  return std::pow(std::pow(z, 2.0) + std::pow(x, 2.0), 0.5);
}




#endif
