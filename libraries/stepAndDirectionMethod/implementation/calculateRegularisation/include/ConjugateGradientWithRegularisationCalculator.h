#pragma once
#include "ConjugateGradientWithRegularisationParametersInput.h"
#include "StepAndDirectionReconstructor.h"

class ConjugateGradientWithRegularisationCalculator : public DirectionCalculator, public StepSizeCalculator
{
private:
    // StepSize part
    dataGrid2D _chiEstimatePrevious;
    dataGrid2D _chiEstimateCurrent;

    dataGrid2D _derivativePrevious;
    dataGrid2D _derivativeCurrent;
    int _iterationNumber;

    // Direction part
    const std::vector<std::complex<double>> &_pData;

    // Inversion part
    ConjugateGradientWithRegularisationParametersInput _cgParametersInput;
    const grid2D &_grid;
    const sources &_sources;
    const receivers &_receivers;
    const frequenciesGroup &_frequencies;
    int _nGridPoints;
    //   dataGrid2D _chiEstimate; //this might just be one of the _chiEstimates above

    //    void calculateRegularisationParameters();
    //    dataGrid2D calculateUpdateRegularisationParameters();
    //    void calculateStepSizeRegularisation();
    //    double const errorFunctional();
    //    void logResidualResults();
    // dataGrid2D calculateRegularisation();
public:
    ConjugateGradientWithRegularisationCalculator(double errorFunctionalScalingFactor, forwardModelInterface *forwardModel,
        ConjugateGradientWithRegularisationParametersInput cgParametersInput, const std::vector<std::complex<double>> &pData);
    virtual ~ConjugateGradientWithRegularisationCalculator();

    double calculateStepSize();
    void updateVariables(const dataGrid2D &chiEstimateCurrent, const dataGrid2D &derivativeCurrent, int iterationNumber);
    dataGrid2D calculateDirection(const dataGrid2D &gradientCurrent, const std::vector<std::complex<double>> &residual) override;
};
