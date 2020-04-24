#pragma once
#include "DirectionCalculator.h"
#include "ReconstructorParameters.h"
#include "StepSizeCalculator.h"
#include "forwardModelInterface.h"

class StepAndDirectionReconstructor
{
private:
    StepSizeCalculator *_chosenStep;
    DirectionCalculator *_chosenDirection;
    forwardModelInterface *_forwardModel;
    const ReconstructorParameters &_directionInput;
    /**  _directionInput contains the specific values for the particular method adopted through the choice a Direction Calculator */
    const grid2D &_grid;

    dataGrid2D calculateNextMove(const dataGrid2D &chiEstimate, const dataGrid2D &direction, double step) const;
    double calculateErrorValue(const std::vector<std::complex<double>> &pData, double eta) const;

public:
    StepAndDirectionReconstructor(
        StepSizeCalculator *chosenStep, DirectionCalculator *chosenDirection, forwardModelInterface *forwardModel, const ReconstructorParameters &directionInput);

    /**
     * @brief reconstruct The main method of the class, where the chosen steps and directions are combined and the reconstruction happens
     * @param pData (not sure, check)
     * @param gInput is the data input for the problem
     * @return
     */
    dataGrid2D reconstruct(const std::vector<std::complex<double>> &pData, genericInput gInput);
};