#include "pressureFieldComplex.h"

PressureFieldComplex::PressureFieldComplex(const Grid2D &grid_) : _grid(grid_), _nGridPoints(_grid.GetNumberOfGridPoints()), _cellVolume(_grid.GetCellVolume())
{
}
