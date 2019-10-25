#include "pressureFieldComplex.h"

pressureFieldComplex::pressureFieldComplex(const grid2D &grid_) : grid(grid_),
                                                                  nGridPoints(grid.GetNumberOfGridPoints()),
                                                                  cellVolume(grid.GetCellVolume()) {}
