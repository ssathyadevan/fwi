#ifndef VOLCOMPLEXFIELD_RECT_2D_H
#define VOLCOMPLEXFIELD_RECT_2D_H

#include <iostream>
#include <string>
#include <functional>
#include <complex>

#include <grid_rect_2D.h>
#include <volField_rect_2D.h>

// Babak 2018 11 02: Detemplating
class volComplexField_rect_2D {


  //const grid_rect_2D<T> &grid;// Babak 2018 10 29: Get rid of template in grid_rect_2D class
  const grid_rect_2D &grid;
  const int nGridPoints;
  const double cellVolume;

  volComplexField_rect_2D(const volComplexField_rect_2D&) = delete;
  volComplexField_rect_2D& operator=(const volComplexField_rect_2D&) = delete;

public:

  //volComplexField_rect_2D(const grid_rect_2D<T> &grid_) :// Babak 2018 10 29: Get rid of template in grid_rect_2D class
  volComplexField_rect_2D(const grid_rect_2D &grid_) :// Babak 2018 10 29: Get rid of template in grid_rect_2D class
    grid(grid_),
    nGridPoints(grid.GetNumberOfGridPoints()),
    cellVolume(grid.GetCellVolume()) {}

  virtual ~volComplexField_rect_2D() {}

  //const grid_rect_2D<T> &GetGrid() const { return grid; }// Babak 2018 10 29: Get rid of template in grid_rect_2D class
  const grid_rect_2D &GetGrid() const { return grid; }

  int GetNumberOfGridPoints() const { return nGridPoints; }
  double getCellVolume() const { return cellVolume; }

  virtual void Zero() = 0;

  virtual void toBuffer(std::complex<double> *buffer) const = 0;
  virtual void fromBuffer(const std::complex<double> *buffer) = 0;

  virtual void SetField(const std::function< std::complex<double>(double, double) > func) = 0;

  virtual double Norm() const = 0;
  virtual double RelNorm() const = 0;

};

#endif /* VOLCOMPLEXFIELD_RECT_2D_H */
