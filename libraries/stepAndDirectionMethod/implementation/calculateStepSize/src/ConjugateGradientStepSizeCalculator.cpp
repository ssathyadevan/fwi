#include "ConjugateGradientStepSizeCalculator.h"

ConjugateGradientStepSizeCalculator::ConjugateGradientStepSizeCalculator(const grid2D &grid, const double initialStepSize) :
    _chiEstimatePrevious(grid), _chiEstimateCurrent(grid), _derivativePrevious(grid), _derivativeCurrent(grid), _initialStepSize(initialStepSize), _iteration(0)
{
}

double ConjugateGradientStepSizeCalculator::calculateStepSize() { return 1.0; }

void ConjugateGradientStepSizeCalculator::updateVariables(const dataGrid2D &, const dataGrid2D &, int) {}
