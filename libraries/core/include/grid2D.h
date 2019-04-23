#ifndef GRID_RECT_2D_H
#define GRID_RECT_2D_H

#include <array>

class grid2D
{

    const std::array<double, 2> x_min;
    const std::array<double, 2> x_max;
    const std::array<int, 2> nx;
    std::array<double, 2> dx;

    int nGridPoints;
    double cellVolume;

    grid2D &operator=(const grid2D &);

public:

    grid2D(const std::array<double, 2> &x_min_,
                 const std::array<double, 2> &x_max_,
                 const std::array<int, 2> &nx_);

    const std::array<int, 2> &GetGridDimensions() const { return nx; }
    const std::array<double, 2> &GetCellDimensions() const { return dx; }
    const std::array<double, 2> &GetGridStart() const { return x_min; }
    const std::array<double, 2> &GetGridEnd() const { return x_max; }

    int GetNumberOfGridPoints() const { return nGridPoints; }
    double GetCellVolume() const { return cellVolume; }
    double GetDomainArea() const { return (x_max[0] - x_min[0]) * (x_max[1] - x_min[1]); }

};

#endif
