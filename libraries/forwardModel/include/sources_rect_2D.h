#ifndef SOURCES_RECT_2D_H
#define SOURCES_RECT_2D_H

#include <cassert>
#include <iostream>
#include <vector>
#include <array>


class Sources_rect_2D
{

public:

  int nSrc;
  std::vector< std::array<double, 2> > xSrc;

  // Place the sources on a linear line from xMin to xMax, including the end points
  Sources_rect_2D(const std::array<double, 2> xMin, const std::array<double, 2> xMax,
                  int nSrc_);

  void Print();

};

#endif
