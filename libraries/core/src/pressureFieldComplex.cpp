#include "pressureFieldComplex.h"

PressureFieldComplex::PressureFieldComplex(const Grid2D &grid_) : grid(grid_),
                                                                  nGridPoints(grid.GetNumberOfGridPoints()),
                                                                  cellVolume(grid.GetCellVolume()) {}
