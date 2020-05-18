#include "ConjugateGradientStepSizeCalculator.h"

ConjugateGradientStepSizeCalculator::ConjugateGradientStepSizeCalculator(const grid2D &grid, const double initialStepSize) :
    _kappaTimesZeta(grid.getNumberOfGridPoints()), _residualVector(grid.getNumberOfGridPoints()), _initialStepSize(initialStepSize)
{
    _nGridPoints = grid.getNumberOfGridPoints();
}

double ConjugateGradientStepSizeCalculator::calculateStepSize()
{
    double alphaNumerator = 0.0;
    double alphaDenominator = 0.0;
    for(int i = 0; i < _nGridPoints; ++i)
    {
        alphaNumerator += std::real(_residualVector[i] * _kappaTimesZeta[i]);
        alphaDenominator += std::real(std::pow(_kappaTimesZeta[i], 2));
    }
    if(alphaDenominator == 0.0)
    {
        throw std::overflow_error("Operator divides by zero");
    }

    return alphaNumerator / alphaDenominator;
}

void ConjugateGradientStepSizeCalculator::updateVariables(const dataGrid2D &, const dataGrid2D &, int, const std::vector<std::complex<double>> &kappaTimesZeta,
    const std::vector<std::complex<double>> &residualVector)
{
    _kappaTimesZeta = kappaTimesZeta;
    _residualVector = residualVector;
}
