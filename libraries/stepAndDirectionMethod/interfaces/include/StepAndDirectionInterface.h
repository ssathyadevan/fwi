#pragma once
#include "DirectionCalculator.h"
#include "DirectionInput.h"
#include "StepSizeCalculator.h"
#include "forwardModelInterface.h"

// bool operator==(StepSizeCalculator *lhs, StepSizeCalculator *rhs) { return lhs->calculateStepSize() == rhs->calculateStepSize(); }

class StepAndDirectionInterface
{
private:
    StepSizeCalculator *_chosenStep;
    DirectionCalculator *_chosenDirection;
    forwardModelInterface *_forwardModel;
    DirectionInput _directionInput;   // this struct should/could be a template
    const grid2D &_grid;

public:
    StepAndDirectionInterface(
        StepSizeCalculator *chosenStep, DirectionCalculator *chosenDirection, forwardModelInterface *forwardModel, const DirectionInput &directionInput);
    //    ~StepAndDirectionInterface() = default;

    dataGrid2D reconstruct(const std::vector<std::complex<double>> &pData, genericInput gInput);
    // gInput is the data inputs, while _directionInput contains the specific values for the particular method adopted through the choice a Direction Calculator

    dataGrid2D calculateNextMove(const dataGrid2D &chiEstimate, const dataGrid2D &direction, double step);
    double functionF(const dataGrid2D chiEstimate, const std::vector<std::complex<double>> &pData, double eta);
    double normSq(const std::vector<std::complex<double>> &pData);

    // StepSizeCalculator getChosenStep() { return *_chosenStep; }
    //   DirectionCalculator getChosenDirection() { return *_chosenDirection; }
    // forwardModelInterface getForwardModel() { return *_forwardModel; } this one gives compile error
    DirectionInput getDirectionInput() { return _directionInput; }
    const grid2D &getGrid() { return _grid; }
};
