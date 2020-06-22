#pragma once
#include "StepSizeCalculator.h"
#include "grid2D.h"

class LinearStepSizeCalculator : public StepSizeCalculator
{
public:
    LinearStepSizeCalculator(double start, double slope);
    virtual ~LinearStepSizeCalculator();

    /**
     * @brief calculateStepSize returns a step size decreasing from _start up to 0.0 as we perform more iterations
     */
    double calculateStepSize() override;

    void updateVariables(
        const core::dataGrid2D &, const core::dataGrid2D &, int iteration, const std::vector<std::complex<double>> &, const std::vector<std::complex<double>> &) override
    {
        _iteration = iteration;
    }

private:
    double _start;
    double _slope;
    int _iteration;
};
