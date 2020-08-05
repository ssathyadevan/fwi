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
                alphaNumerator += std::real(conj(_residualVector[i]) * _kappaTimesDirection[i]);
                // the proper way to compute the squared norm of a complex number X is conj(X)*X
                alphaDenominator += std::real(conj(_kappaTimesDirection[i]) * _kappaTimesDirection[i]);
            }
            if(alphaDenominator == 0.0)
            {
                throw std::overflow_error("Operator divides by zero");
            }
            return alphaNumerator / alphaDenominator;
        }

        void ConjugateGradientStepSizeCalculator::updateVariables(const core::dataGrid2D &, const core::dataGrid2D &, int,
            const std::vector<std::complex<double>> &kappaTimesDirection, const std::vector<std::complex<double>> &residualVector)
        {
            _kappaTimesDirection = kappaTimesDirection;
            _residualVector = residualVector;
        }
    }   // namespace inversionMethods
}   // namespace fwi
