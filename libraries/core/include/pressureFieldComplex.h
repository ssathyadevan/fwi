#ifndef VOLCOMPLEXFIELD_RECT_2D_H
#define VOLCOMPLEXFIELD_RECT_2D_H

#include <iostream>
#include <string>
#include <functional>
#include <complex>
#include <grid2D.h>
#include <pressureField.h>

class pressureFieldComplex
{

  const grid2D &grid;
  const int nGridPoints;
  const double cellVolume;
  pressureFieldComplex(const pressureFieldComplex&) = delete;
  pressureFieldComplex& operator=(const pressureFieldComplex&) = delete;

public:

  pressureFieldComplex(const grid2D &grid_);

  virtual ~pressureFieldComplex() {}

  const grid2D &GetGrid() const { return grid; }

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
