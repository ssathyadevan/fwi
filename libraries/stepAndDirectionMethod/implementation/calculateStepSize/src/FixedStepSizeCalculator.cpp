#include "FixedStepSizeCalculator.h"

FixedStepSizeCalculator::FixedStepSizeCalculator(double size) : _size(size) {}

FixedStepSizeCalculator::~FixedStepSizeCalculator() {}

double FixedStepSizeCalculator::calculateStepSize() { return _size; }

void FixedStepSizeCalculator::setStepSize(double size) { _size = size; }

void FixedStepSizeCalculator::setCurrentVariables(const dataGrid2D &, const dataGrid2D &) {}

void FixedStepSizeCalculator::updateVariables(const dataGrid2D &, const dataGrid2D &) {}
