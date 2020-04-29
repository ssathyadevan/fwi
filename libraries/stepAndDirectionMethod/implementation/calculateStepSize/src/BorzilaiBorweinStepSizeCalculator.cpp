#include "BorzilaiBorweinStepSizeCalculator.h"
// the Borzilai-Borwein is the most common choice for the step computation of the Gradient Descent algorithm,
// see https://en.wikipedia.org/wiki/Gradient_descent

BorzilaiBorweinStepSizeCalculator::BorzilaiBorweinStepSizeCalculator(const grid2D &grid, const double initialStepSize) :
    _chiEstimatePrevious(grid), _chiEstimateCurrent(grid), _derivativePrevious(grid), _derivativeCurrent(grid), _initialStepSize(initialStepSize), _iteration(0)
{
}

BorzilaiBorweinStepSizeCalculator::~BorzilaiBorweinStepSizeCalculator() {}

double BorzilaiBorweinStepSizeCalculator::calculateStepSize()
{
    if(_iteration == 0)
    {
        return _initialStepSize;
    }
    const int nGridPoints = _chiEstimateCurrent.getNumberOfGridPoints();

    const std::vector<double> &chiDataCurrent = _chiEstimateCurrent.getData();
    const std::vector<double> &chiDataPrevious = _chiEstimatePrevious.getData();
    const std::vector<double> &derivativeCurrent = _derivativeCurrent.getData();
    const std::vector<double> &derivativePrevious = _derivativePrevious.getData();

    std::vector<double> chiDataDifference(nGridPoints, 0.0);
    for(int i = 0; i < nGridPoints; ++i)
    {
        chiDataDifference[i] = chiDataCurrent[i] - chiDataPrevious[i];
    }

    std::vector<double> derivativeDifference(nGridPoints, 0.0);
    for(int i = 0; i < nGridPoints; ++i)
    {
        derivativeDifference[i] = derivativeCurrent[i] - derivativePrevious[i];
    }

    double gammaNumerator = 0.0;
    for(int i = 0; i < nGridPoints; ++i)
    {
        gammaNumerator += chiDataDifference[i] * derivativeDifference[i];
    }
    gammaNumerator = fabs(gammaNumerator);

    double gammaDenominator = 0.0;
    for(int i = 0; i < nGridPoints; ++i)
    {
        gammaDenominator += pow(derivativeDifference[i], 2);
    }

    if(gammaDenominator == 0.0)
    {
        throw std::overflow_error("Operator divides by zero");
    }

    double gamma = gammaNumerator / gammaDenominator;
    return gamma;
}

void BorzilaiBorweinStepSizeCalculator::updateVariables(const dataGrid2D &chiEstimateCurrent, const dataGrid2D &derivativeCurrent, int iteration)
{
    _chiEstimatePrevious = _chiEstimateCurrent;
    _derivativePrevious = _derivativeCurrent;
    _chiEstimateCurrent = chiEstimateCurrent;
    _derivativeCurrent = derivativeCurrent;
    _iteration = iteration;
}
