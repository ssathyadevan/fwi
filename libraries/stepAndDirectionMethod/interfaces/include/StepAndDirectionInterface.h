#pragma once
#include "DirectionCalculator.h"
#include "DirectionInput.h"
#include "forwardModelInterface.h"

class StepCalculator
{
public:
    double calculateStep(dataGrid2D chiEstimatePrevious, dataGrid2D chiEstimateCurrent, dataGrid2D directionPrevious, dataGrid2D directionCurrent)
    {
        chiEstimatePrevious.zero();
        chiEstimateCurrent.zero();
        directionPrevious.zero();
        directionCurrent.zero();
        return 1.0;
    }
};

class StepAndDirectionInterface
{
private:
    StepCalculator *_chosenStep;
    DirectionCalculator *_chosenDirection;
    DirectionInput _directionInput;   // this struct should be a template
    forwardModelInterface *_forwardModel;

    const grid2D &_grid;
    const sources &_src;
    const receivers &_recv;
    const frequenciesGroup _freq;

public:
    StepAndDirectionInterface(
        StepCalculator *chosenStep, DirectionCalculator *chosenDirection, forwardModelInterface *forwardModel, const DirectionInput &directionInput);
    //    ~StepAndDirectionInterface() = default;

    dataGrid2D reconstruct(const std::vector<std::complex<double>> &pData, genericInput gInput);
    // gInput is the data inputs, while _directionInput contains the specific values for the particular method adopted through the choice a 'Direction'

    dataGrid2D calculateNextMove(dataGrid2D chiEstimate, const dataGrid2D &direction, double step);
    double functionF(const dataGrid2D chiEstimate, const std::vector<std::complex<double>> &pData, double eta);
    double normSq(const std::vector<std::complex<double>> &pData);
};
