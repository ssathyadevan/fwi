#include "volComplexField_rect_2D.h"

volComplexField_rect_2D::volComplexField_rect_2D(const grid_rect_2D &grid_) :// Babak 2018 10 29: Get rid of template in grid_rect_2D class
  grid(grid_),
  nGridPoints(grid.GetNumberOfGridPoints()),
  cellVolume(grid.GetCellVolume()) {}
