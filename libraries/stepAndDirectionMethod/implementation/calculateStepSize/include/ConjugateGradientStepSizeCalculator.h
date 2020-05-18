#pragma once
#include "StepSizeCalculator.h"

class ConjugateGradientStepSizeCalculator : public StepSizeCalculator
{
private:
    std::vector<std::complex<double>> _kappaTimesDirection;
    std::vector<std::complex<double>> _residualVector;

    const double _initialStepSize;
    int _nGridPoints;

public:
    ConjugateGradientStepSizeCalculator(const grid2D &grid, const double initialStepSize);
    virtual ~ConjugateGradientStepSizeCalculator() {}

    /**
     * @brief calculateStepSize This method returns the computation described in eq. 2.17 of
     * doc/BackGroundInfo/phd-Peter-Haffinger-seismic-broadband-full-waveform-inversion.pdf
     * @return the step optimized for the standard conjugate gradient withouth regularisation
     */
    double calculateStepSize();

    /**
     * @brief updateVariables Updates the quantities that are needed to compute the optimized step
     * @param kappaTimesDirection is the inner product between _kappa from the forwardModel and the computed current direction (= eta)
     * @param residualVector is the residual of the previous iteration. For the first iteration, the initial residual is used instead.
     */
    void updateVariables(const dataGrid2D &, const dataGrid2D &, int, const std::vector<std::complex<double>> &kappaTimesDirection,
        const std::vector<std::complex<double>> &residualVector);
};
