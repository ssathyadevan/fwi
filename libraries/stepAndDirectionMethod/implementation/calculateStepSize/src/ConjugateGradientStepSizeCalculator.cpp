#include "ConjugateGradientStepSizeCalculator.h"

namespace fwi
{
    namespace inversionMethods
    {
        ConjugateGradientStepSizeCalculator::ConjugateGradientStepSizeCalculator(const core::grid2D &grid, const double initialStepSize)
            : _kappaTimesDirection(grid.getNumberOfGridPoints())
            , _residualVector(grid.getNumberOfGridPoints())
            , _initialStepSize(initialStepSize)
        {
            _nGridPoints = grid.getNumberOfGridPoints();
        }

        double ConjugateGradientStepSizeCalculator::calculateStepSize()
        {
            double alphaNumerator = 0.0;
            double alphaDenominator = 0.0;

            for(int i = 0; i < _nGridPoints; ++i)
            {
                alphaNumerator += std::real(_residualVector[i] * _kappaTimesDirection[i]);
                alphaDenominator += std::norm(_kappaTimesDirection[i]);
            }
            if(alphaDenominator == 0.0)
            {
                throw std::overflow_error("Operator divides by zero");
            }
            // According to 1_ProjectDescription.pdf and to computations the - sign should not be there, but if removed it does not converge.
            return -alphaNumerator / alphaDenominator;
        }

        void ConjugateGradientStepSizeCalculator::updateVariables(const core::dataGrid2D &, const core::dataGrid2D &, int,
            const std::vector<std::complex<double>> &kappaTimesDirection, const std::vector<std::complex<double>> &residualVector)
        {
            _kappaTimesDirection = kappaTimesDirection;
            _residualVector = residualVector;
        }
    }   // namespace inversionMethods
}   // namespace fwi
