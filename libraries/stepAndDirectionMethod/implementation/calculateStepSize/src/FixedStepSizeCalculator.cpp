#include "FixedStepSizeCalculator.h"

FixedStepSizeCalculator::FixedStepSizeCalculator(double size) : _size(size) {}

FixedStepSizeCalculator::~FixedStepSizeCalculator() {}

double FixedStepSizeCalculator::calculateStepSize() { return _size; }

void FixedStepSizeCalculator::setStepSize(double size) { _size = size; }
