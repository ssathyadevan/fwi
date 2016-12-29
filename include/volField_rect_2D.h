#ifndef VOLFIELD_RECT_2D_H
#define VOLFIELD_RECT_2D_H

#include <iostream>
#include <string>
#include <functional>

#include <grid_rect_2D.h>

template <class T>
class volField_rect_2D {

protected:

  const grid_rect_2D<T> &grid;
  const int nGridPoints;
  const T cellVolume;

  volField_rect_2D(const volField_rect_2D<T>&) = delete;
  volField_rect_2D<T>& operator=(const volField_rect_2D<T>&) = delete;

public:

  volField_rect_2D(const grid_rect_2D<T> &grid_) :
    grid(grid_),
    nGridPoints(grid.GetNumberOfGridPoints()),
    cellVolume(grid.GetCellVolume()) {}

  virtual ~volField_rect_2D() {}

  const grid_rect_2D<T> &GetGrid() const { return grid; }
  int GetNumberOfGridPoints() const { return nGridPoints; }
  T getCellVolume() const { return cellVolume; }

  virtual void Zero() = 0;
  virtual void Square() = 0;
  virtual void Sqrt() = 0;
  virtual void Random() = 0;

  virtual void toBuffer(T *buffer) const = 0;
  virtual void fromBuffer(const T *buffer) = 0;

  virtual void toFile(const std::string &fileName) const = 0;

  virtual void SetField(const std::function< T(T,T) > func) = 0;

  virtual T Norm() const = 0;
  virtual T RelNorm() const = 0;
  virtual T Summation() const = 0;

};

#endif /* VOLFIELD_RECT_2D_H */
