#include "DirectionCalculator.h"

DirectionCalculator::DirectionCalculator(const grid2D &grid, double eta) : _grid(grid), _eta(eta), _direction(grid) {}

DirectionCalculator::~DirectionCalculator() {}
