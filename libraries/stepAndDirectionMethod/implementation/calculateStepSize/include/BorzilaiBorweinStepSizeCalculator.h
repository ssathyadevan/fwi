#pragma once

#include "StepSizeCalculator.h"

class BorzilaiBorweinStepSizeCalculator : public StepSizeCalculator
{
private:
    std::vector<double> _chiEstimatePrevious;
    std::vector<double> _chiEstimateCurrent;

    std::vector<double> _derivativePrevious;
    std::vector<double> _derivativeCurrent;

    const double _initialStepSize;
    int _iteration;
    int _nGridPoints;

public:
    BorzilaiBorweinStepSizeCalculator(const grid2D &grid, const double initialStepSize);
    virtual ~BorzilaiBorweinStepSizeCalculator();

    double calculateStepSize();
    void updateVariables(const dataGrid2D &chiEstimateCurrent, const dataGrid2D &derivativeCurrent, int iteration);
};
