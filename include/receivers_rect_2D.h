#ifndef RECEIVERS_RECT_2D_H
#define RECEIVERS_RECT_2D_H

#include <iostream>
#include <vector>
#include <array>

#include "sources_rect_2D.h"

template <typename T>
class Receivers_rect_2D {

	Receivers_rect_2D<T>& operator=(const Receivers_rect_2D<T>&);

public:

  const int nRecv;
  const std::vector< std::array<T, 2> > xRecv;

  Receivers_rect_2D(const Sources_rect_2D<T> &src) : nRecv(src.nSrc), xRecv(src.xSrc) {}

  void Print() {
    std::cout << "Total number is receivers is " << nRecv << ". Positions:" << std::endl;

    for(int i=0; i<nRecv; i++) {
      std::cout << "x = (" << xRecv[i][0] << ", " << xRecv[i][1] << ")" << std::endl;
    }

    std::cout << std::endl;
  }

};

#endif
