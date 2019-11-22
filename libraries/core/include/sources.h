#pragma once

#include <cassert>
#include <iostream>
#include <vector>
#include <array>


class Sources
{

public:

  int nSrc;
  std::vector< std::array<double, 2> > xSrc;
  // Place the sources on a linear line from xMin to xMax, including the end points
  Sources(const std::array<double, 2> xMin, const std::array<double, 2> xMax,
                  int nSrc_);

  void Print();

private:
  std::array<double, 2> calculate_distance(const std::array<double, 2> xMin, const std::array<double, 2> xMax);
};


