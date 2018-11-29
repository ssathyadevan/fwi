#include "volComplexField_rect_2D.h"

volComplexField_rect_2D::volComplexField_rect_2D(const grid_rect_2D &grid_) :
  grid(grid_),
  nGridPoints(grid.GetNumberOfGridPoints()),
  cellVolume(grid.GetCellVolume()) {}
