#pragma once

#include <iostream>
#include <vector>
#include <array>

#include "sources.h"

class receivers
{

    receivers& operator=(const receivers&);

public:

  const int nRecv;
  std::vector< std::array<double, 2> > xRecv;

  receivers(const std::array<double, 2> xMin, const std::array<double, 2> xMax, int nRecv_);

  void Print();

};

