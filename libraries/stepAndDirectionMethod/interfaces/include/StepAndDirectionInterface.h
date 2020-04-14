#pragma once
#include "DirectionCalculator.h"
#include "DirectionInput.h"
#include "forwardModelInterface.h"

class StepCalculator   // to be  replaced with Alex's classes
{
public:
    double _step;

    StepCalculator() { _step = 1.0; }
    StepCalculator(double step) { _step = step; }

    double calculateStep(dataGrid2D chiEstimatePrevious, dataGrid2D chiEstimateCurrent, dataGrid2D directionPrevious, dataGrid2D directionCurrent)
    {
        chiEstimatePrevious.zero();
        chiEstimateCurrent.zero();
        directionPrevious.zero();
        directionCurrent.zero();
        return 1.0;
    }
};
// bool operator==(StepCalculator lhs, StepCalculator rhs) { return lhs._step == rhs._step; }

class StepAndDirectionInterface
{
private:
    StepCalculator *_chosenStep;
    DirectionCalculator *_chosenDirection;
    forwardModelInterface *_forwardModel;
    DirectionInput _directionInput;   // this struct should/could be a template
    const grid2D &_grid;

public:
    StepAndDirectionInterface(
        StepCalculator *chosenStep, DirectionCalculator *chosenDirection, forwardModelInterface *forwardModel, const DirectionInput &directionInput);
    //    ~StepAndDirectionInterface() = default;

    dataGrid2D reconstruct(const std::vector<std::complex<double>> &pData, genericInput gInput);
    // gInput is the data inputs, while _directionInput contains the specific values for the particular method adopted through the choice a Direction Calculator

    dataGrid2D calculateNextMove(dataGrid2D chiEstimate, const dataGrid2D &direction, double step);
    double functionF(const dataGrid2D chiEstimate, const std::vector<std::complex<double>> &pData, double eta);
    double normSq(const std::vector<std::complex<double>> &pData);

    StepCalculator getChosenStep() { return *_chosenStep; }
    DirectionCalculator getChosenDirection() { return *_chosenDirection; }
    // forwardModelInterface getForwardModel() { return *_forwardModel; } this one gives compile error
    DirectionInput getDirectionInput() { return _directionInput; }
    const grid2D &getGrid() { return _grid; }
};
