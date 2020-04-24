#include "FixedStepSizeCalculator.h"

FixedStepSizeCalculator::FixedStepSizeCalculator(double size) : _size(size) {}

FixedStepSizeCalculator::~FixedStepSizeCalculator() {}

double FixedStepSizeCalculator::calculateStepSize() { return _size; }

void FixedStepSizeCalculator::updateVariables(const dataGrid2D &, const dataGrid2D &, int) {}