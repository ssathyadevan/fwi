#pragma once

#include <iostream>
#include <vector>
#include <array>

#include "sources.h"

class Receivers
{

  Receivers &operator=(const Receivers &); // unused?

public:
  const int nRecv;
  std::vector<std::array<double, 2>> xRecv;

  Receivers(const std::array<double, 2> xMin, const std::array<double, 2> xMax, int nRecv_);

  void Print();

private:
  std::array<double, 2> calculate_distance(const std::array<double, 2> xMin, const std::array<double, 2> xMax);
};
