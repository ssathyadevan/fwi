#include "BorzilaiBorweinStepSizeCalculator.h"
// the Borzilai-Borwein is the most common choice for the step computation of the Gradient Descent algorithm,
// see https://en.wikipedia.org/wiki/Gradient_descent
namespace inversionMethods
{
    BorzilaiBorweinStepSizeCalculator::BorzilaiBorweinStepSizeCalculator(const core::grid2D &grid, const double initialStepSize)
        : _chiEstimatePrevious(grid.getNumberOfGridPoints(), 0.0)
        , _chiEstimateCurrent(grid.getNumberOfGridPoints(), 0.0)
        , _derivativePrevious(grid.getNumberOfGridPoints(), 0.0)
        , _derivativeCurrent(grid.getNumberOfGridPoints(), 0.0)
        , _initialStepSize(initialStepSize)
        , _iteration(0)
    {
        _nGridPoints = grid.getNumberOfGridPoints();
    }

    BorzilaiBorweinStepSizeCalculator::~BorzilaiBorweinStepSizeCalculator() {}

    double BorzilaiBorweinStepSizeCalculator::calculateStepSize()
    {
        if(_iteration == 0)
        {
            return _initialStepSize;
        }

        std::vector<double> chiDataDifference(_nGridPoints, 0.0);
        for(int i = 0; i < _nGridPoints; ++i)
        {
            chiDataDifference[i] = _chiEstimateCurrent[i] - _chiEstimatePrevious[i];
        }

        std::vector<double> derivativeDifference(_nGridPoints, 0.0);
        for(int i = 0; i < _nGridPoints; ++i)
        {
            derivativeDifference[i] = _derivativeCurrent[i] - _derivativePrevious[i];
        }

        double gammaNumerator = 0.0;
        for(int i = 0; i < _nGridPoints; ++i)
        {
            gammaNumerator += chiDataDifference[i] * derivativeDifference[i];
        }
        gammaNumerator = fabs(gammaNumerator);

        double gammaDenominator = 0.0;
        for(int i = 0; i < _nGridPoints; ++i)
        {
            gammaDenominator += pow(derivativeDifference[i], 2);
        }

        if(gammaDenominator == 0.0)
        {
            throw std::overflow_error("Operator divides by zero");
        }

        return gammaNumerator / gammaDenominator;
    }

    void BorzilaiBorweinStepSizeCalculator::updateVariables(const core::dataGrid2D &chiEstimateCurrent, const core::dataGrid2D &derivativeCurrent,
        int iteration, const std::vector<std::complex<double>> &, const std::vector<std::complex<double>> &)
    {
        _chiEstimatePrevious = _chiEstimateCurrent;
        _derivativePrevious = _derivativeCurrent;
        _chiEstimateCurrent = chiEstimateCurrent.getData();
        _derivativeCurrent = derivativeCurrent.getData();
        _iteration = iteration;
    }
}   // namespace inversionMethods
