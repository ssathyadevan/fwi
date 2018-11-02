#ifndef SOURCES_RECT_2D_H
#define SOURCES_RECT_2D_H

#include <cassert>
#include <iostream>
#include <vector>
#include <array>

//Babak 2018 11 02: got rid of templates
class Sources_rect_2D {

public:

  int nSrc;
  std::vector< std::array<double, 2> > xSrc;

  // Place the sources on a linear line from xMin to xMax, including the end points
  Sources_rect_2D(const std::array<double, 2> xMin, const std::array<double, 2> xMax, int nSrc_) : nSrc(nSrc_), xSrc() {

    assert(nSrc > 1);

    std::array<double, 2> dx;
    for(int j=0; j<2; j++) {
      dx[j] = (xMax[j] - xMin[j]) / (nSrc - 1);
    }

    for(int i=0; i<nSrc; i++) {
      std::array<double, 2> src;
      for(int j=0; j<2; j++) {
        src[j] = xMin[j] + i * dx[j];
      }
      xSrc.push_back(src);
    }
  }

  void Print() {

    std::cout << "Total number is sources is " << nSrc << ". Positions:" << std::endl;

    for(int i=0; i<nSrc; i++) {
      std::cout << "x = (" << xSrc[i][0] << ", " << xSrc[i][1] << ")" << std::endl;
    }

    std::cout << std::endl;
  }

};

#endif

//template <typename T>
//class Sources_rect_2D {

//public:

//  int nSrc;
//  std::vector< std::array<T, 2> > xSrc;

//  // Place the sources on a linear line from xMin to xMax, including the end points
//  Sources_rect_2D(const std::array<T, 2> xMin, const std::array<T, 2> xMax, int nSrc_) : nSrc(nSrc_), xSrc() {

//    assert(nSrc > 1);

//    std::array<T, 2> dx;
//    for(int j=0; j<2; j++) {
//      dx[j] = (xMax[j] - xMin[j]) / (nSrc - 1);
//    }

//    for(int i=0; i<nSrc; i++) {
//      std::array<T, 2> src;
//      for(int j=0; j<2; j++) {
//        src[j] = xMin[j] + i * dx[j];
//      }
//      xSrc.push_back(src);
//    }

//  }

//  void Print() {

//    std::cout << "Total number is sources is " << nSrc << ". Positions:" << std::endl;

//    for(int i=0; i<nSrc; i++) {
//      std::cout << "x = (" << xSrc[i][0] << ", " << xSrc[i][1] << ")" << std::endl;
//    }

//    std::cout << std::endl;
//  }

//};

//#endif
