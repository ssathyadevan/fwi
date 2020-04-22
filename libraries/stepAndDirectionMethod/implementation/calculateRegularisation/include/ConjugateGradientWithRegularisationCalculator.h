#pragma once
#include "StepAndDirectionReconstructor.h"

class ConjugateGradientWithRegularisationCalculator : public DirectionCalculator, public StepSizeCalculator
{
private:
    int _regularisationIterations;

    double tolerance;
    // these are for the regularisation slope
    double start;
    double slope;

    //    const grid2D &_grid;
    //    const sources &_sources;
    //    const receivers &_receivers;
    //    const frequenciesGroup &_frequencies;

    //   dataGrid2D _chiEstimate;
    //    void calculateRegularisationParameters();
    //    dataGrid2D calculateUpdateRegularisationParameters();
    //    void calculateStepSizeRegularisation();
    //    double const errorFunctional();
    //    void logResidualResults();
    // dataGrid2D calculateRegularisation();
public:
    ConjugateGradientWithRegularisationCalculator(double errorFunctionalScalingFactor, forwardModelInterface *forwardModel, int regularisationIterations);
    virtual ~ConjugateGradientWithRegularisationCalculator();

    double calculateStepSize();
    //    void updateVariables(const dataGrid2D &chiEstimateCurrent, const dataGrid2D &derivativeCurrent, int);
    dataGrid2D calculateDirection(const dataGrid2D &chi, const std::vector<std::complex<double>> &residual) override;
};
