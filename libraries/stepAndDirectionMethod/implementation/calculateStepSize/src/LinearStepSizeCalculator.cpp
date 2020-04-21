#include "LinearStepSizeCalculator.h"

LinearStepSizeCalculator::LinearStepSizeCalculator(double start, double slope) : _start(start), _slope(slope) { _iteration = 0; }

double LinearStepSizeCalculator::calculateStepSize() { return _start + _iteration * _slope; }

void LinearStepSizeCalculator::updateVariables(const dataGrid2D &, const dataGrid2D &, int iteration) { _iteration = iteration; }
