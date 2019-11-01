#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <complex>
#include <grid2D.h>
#include <pressureField.h>

class PressureFieldComplex
{

  const Grid2D &grid;
  const int nGridPoints;
  const double cellVolume;
  PressureFieldComplex(const PressureFieldComplex&) = delete;
  PressureFieldComplex& operator=(const PressureFieldComplex&) = delete;

public:

  PressureFieldComplex(const Grid2D &grid_);

  virtual ~PressureFieldComplex() {}

  const Grid2D &GetGrid() const { return grid; }

  int GetNumberOfGridPoints() const { return nGridPoints; }
  double getCellVolume() const { return cellVolume; }

  virtual void Zero() = 0;

  virtual void toBuffer(std::complex<double> *buffer) const = 0;
  virtual void fromBuffer(const std::complex<double> *buffer) = 0;

  virtual void SetField(const std::function< std::complex<double>(double, double) > func) = 0;

  virtual double Norm() const = 0;
  virtual double RelNorm() const = 0;

};

