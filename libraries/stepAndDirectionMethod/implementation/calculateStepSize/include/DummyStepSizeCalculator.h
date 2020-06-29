#pragma once

#include "StepSizeCalculator.h"

class DummyStepSizeCalculator : public StepSizeCalculator
{
public:
    DummyStepSizeCalculator();                       // default constructor
    DummyStepSizeCalculator(double dummyStepSize);   // non-default constructor
    virtual ~DummyStepSizeCalculator();

    /**
     * @brief calculateStepSize is called by StepAndDirectionReconstructor::Reconstruct
     * @return is the StepSize optimized following the chosen algorithm implementation and the quantities that have been updated through updateVariables()
     */
    double calculateStepSize();

    /**
     * @brief updateVariables takes a number of variables from the method StepAndDirectionReconstructor::Reconstruct and updates the variables that will be
     * needed by calculateStepSize(). If some variables are not needed, their names (but not the types) should be removed from the input in the declaration (in
     * this file) and the definition (in the .cpp file) and remove the (void)variableName as well.
     * @param chiEstimateCurrent is the current estimate we have of the solution
     * @param derivativeCurrent is the current derivative of the cost function computed in chiEstimateCurrent
     * @param iteration is the number of iterations done so far
     * @param kappaTimesDirection in the documentation represents the quantity Kappa times Chi
     */
    void updateVariables(const dataGrid2D &chiEstimateCurrent, const dataGrid2D &derivativeCurrent, int iteration,
        const std::vector<std::complex<double>> &kappaTimesDirection, const std::vector<std::complex<double>> &);

private:
    double _dummyStepSize;
};
