#include "FixedStepSizeCalculator.h"

FixedStepSizeCalculator::FixedStepSizeCalculator(double size) : _size(size) {}

FixedStepSizeCalculator::~FixedStepSizeCalculator() {}

double FixedStepSizeCalculator::calculateStepSize() { return _size; }

void FixedStepSizeCalculator::updateVariables(
    const core::dataGrid2D &, const core::dataGrid2D &, int, const std::vector<std::complex<double>> &, const std::vector<std::complex<double>> &)
{
}
