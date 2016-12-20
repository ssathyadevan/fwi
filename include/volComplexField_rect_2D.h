#ifndef VOLCOMPLEXFIELD_RECT_2D_H
#define VOLCOMPLEXFIELD_RECT_2D_H

#include <iostream>
#include <string>
#include <functional>
#include <complex>

#include <grid_rect_2D.h>
#include <volField_rect_2D.h>

template <class T>
class volComplexField_rect_2D {


  const grid_rect_2D<T> &grid;
  const int nGridPoints;
  const T cellVolume;

  volComplexField_rect_2D(const volComplexField_rect_2D<T>&) = delete;
  volComplexField_rect_2D<T>& operator=(const volComplexField_rect_2D<T>&) = delete;

public:

  volComplexField_rect_2D(const grid_rect_2D<T> &grid_) :
    grid(grid_),
    nGridPoints(grid.GetNumberOfGridPoints()),
    cellVolume(grid.GetCellVolume()) {}

  virtual ~volComplexField_rect_2D() {}

  const grid_rect_2D<T> &GetGrid() const { return grid; }
  int GetNumberOfGridPoints() const { return nGridPoints; }
  T getCellVolume() const { return cellVolume; }

  virtual void Zero() = 0;

  virtual void toBuffer(std::complex<T> *buffer) const = 0;
  virtual void fromBuffer(const std::complex<T> *buffer) = 0;

  virtual void SetField(const std::function< std::complex<T>(T, T) > func) = 0;

  virtual T Norm() const = 0;
  virtual T RelNorm() const = 0;

};

#endif /* VOLCOMPLEXFIELD_RECT_2D_H */
