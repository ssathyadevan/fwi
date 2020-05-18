#pragma once
#include "StepSizeCalculator.h"
// this class performs the computation described in eq. 2.17 of doc/BackGroundInfo/phd-Peter-Haffinger-seismic-broadband-full-waveform-inversion.pdf

class ConjugateGradientStepSizeCalculator : public StepSizeCalculator
{
private:
    std::vector<std::complex<double>> _kappaTimesZeta;
    std::vector<std::complex<double>> _residualVector;

    std::vector<double> _zetaCurrent;

    const double _initialStepSize;   // this is never used
    int _iteration;
    int _nGridPoints;

public:
    ConjugateGradientStepSizeCalculator(const grid2D &grid, const double initialStepSize);
    virtual ~ConjugateGradientStepSizeCalculator() {}

    double calculateStepSize();
    void updateVariables(const dataGrid2D &, const dataGrid2D &zetaCurrent, int iteration, const std::vector<std::complex<double>> &kappaTimesZeta,
        const std::vector<std::complex<double>> &residualVector);
};
