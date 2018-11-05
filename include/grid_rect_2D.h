#ifndef GRID_2D_RECT_H
#define GRID_2D_RECT_H

#include <array>

/*
    Babak 2018 10 29: Got rid of template
    Babak 2018 10 29: Gott rid of inheritence
    Babak 2018 11 05: Detemplating the class
*/

class grid_rect_2D{

    const std::array<double, 2> x_min;
    const std::array<double, 2> x_max;
    const std::array<int, 2> nx;
    std::array<double, 2> dx;

    int nGridPoints;
    double cellVolume;

    grid_rect_2D &operator=(const grid_rect_2D &);

public:

    grid_rect_2D(const std::array<double, 2> &x_min_,
                 const std::array<double, 2> &x_max_,
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
    const std::array<double, 2> &GetCellDimensions() const { return dx; }
    const std::array<double, 2> &GetGridStart() const { return x_min; }

    int GetNumberOfGridPoints() const { return nGridPoints; }
    double GetCellVolume() const { return cellVolume; }
    double GetDomainArea() const { return (x_max[0] - x_min[0]) * (x_max[1] - x_min[1]); }

};

#endif

/*
  Babak 2018 10 29: Get rid of template
  #include <grid.h>
  template <class T>
  class grid_rect_2D : public Grid<T> {//Babak 2018 10 29: Get rid of inheritence

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
  T GetDomainArea() const { return (x_max[0] - x_min[0]) * (x_max[1] - x_min[1]); }

};

#endif
*/
