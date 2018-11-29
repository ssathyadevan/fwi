#ifndef RECEIVERS_RECT_2D_H
#define RECEIVERS_RECT_2D_H

#include <iostream>
#include <vector>
#include <array>

#include "sources_rect_2D.h"

class Receivers_rect_2D
{

    Receivers_rect_2D& operator=(const Receivers_rect_2D&);

public:

  const int nRecv;
  const std::vector< std::array<double, 2> > xRecv;

  Receivers_rect_2D(const Sources_rect_2D &src);

  void Print();

};

#endif
