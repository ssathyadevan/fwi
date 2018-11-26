#include "helper.h"

#include <cmath>
#include <iostream>
#include <string>

double dist(double x, double z) {
  return std::pow(std::pow(z, 2.0) + std::pow(x, 2.0), 0.5);
}
