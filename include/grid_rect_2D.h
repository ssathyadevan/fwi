#ifndef GRID_2D_RECT_H
#define GRID_2D_RECT_H

#include <array>

#include <grid.h>

template <class T>
class grid_rect_2D : public Grid<T> {

  const std::array<T, 2> x_min;
  const std::array<T, 2> x_max;
  const std::array<int, 2> nx;
  std::array<T, 2> dx;

  int nGridPoints;
  T cellVolume;

  grid_rect_2D<T> &operator=(const grid_rect_2D<T> &);

public:

  grid_rect_2D(const std::array<T, 2> &x_min_,
               const std::array<T, 2> &x_max_,
               const std::array<int, 2> &nx_) :
    x_min(x_min_),
    x_max(x_max_),
    nx(nx_), dx(), nGridPoints(), cellVolume() {

    // Fix this with std::transform so that we can move it to the
    // initializer list and make all members const
    dx[0] = (x_max[0] - x_min[0]) / nx[0];
    dx[1] = (x_max[1] - x_min[1]) / nx[1];

    cellVolume = dx[0] * dx[1];
    nGridPoints = nx[0] * nx[1];

  }

  const std::array<int, 2> &GetGridDimensions() const { return nx; }
  const std::array<T, 2> &GetCellDimensions() const { return dx; }
  const std::array<T, 2> &GetGridStart() const { return x_min; }

  int GetNumberOfGridPoints() const { return nGridPoints; }
  T GetCellVolume() const { return cellVolume; }

};

#endif
