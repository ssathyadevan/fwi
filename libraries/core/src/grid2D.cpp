#include "grid2D.h"

grid2D::grid2D(const std::array<double, 2> &x_min_,
               const std::array<double, 2> &x_max_,
               const std::array<int, 2> &nx_) : x_min(x_min_),
                                                x_max(x_max_),
                                                nx(nx_), dx(), nGridPoints(), cellVolume()
{
    // Fix this with std::transform so that we can move it to the
    // initializer list and make all members const
    dx[0] = (x_max[0] - x_min[0]) / static_cast<double>(nx[0]);
    dx[1] = (x_max[1] - x_min[1]) / static_cast<double>(nx[1]);

    cellVolume = dx[0] * dx[1];
    nGridPoints = nx[0] * nx[1]; // Should be renamed to nCells.
}
