#ifndef RECEIVERS_RECT_2D_H
#define RECEIVERS_RECT_2D_H

#include <iostream>
#include <vector>
#include <array>

#include "sources.h"

class receivers
{

    receivers& operator=(const receivers&);

public:

  const int nRecv;
  const std::vector< std::array<double, 2> > xRecv;

  receivers(const sources &src);

  void Print();

};

#endif
