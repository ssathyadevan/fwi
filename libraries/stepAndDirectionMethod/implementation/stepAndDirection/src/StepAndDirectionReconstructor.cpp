#include "StepAndDirectionReconstructor.h"
#include "progressBar.h"

StepAndDirectionReconstructor::StepAndDirectionReconstructor(
    StepSizeCalculator *chosenStep, DirectionCalculator *chosenDirection, forwardModelInterface *forwardModel, const ReconstructorParameters &directionInput) :
    _chosenStep(chosenStep),
    _chosenDirection(chosenDirection), _forwardModel(forwardModel), _directionInput(directionInput), _grid(forwardModel->getGrid())
{
}

dataGrid2D StepAndDirectionReconstructor::reconstruct(const std::vector<std::complex<double>> &pData, genericInput gInput)
{
    progressBar bar(_directionInput.maxIterationsNumber);

    std::ofstream file(gInput.outputLocation + gInput.runName + "Residual.log");

    double step = 0.0;

    double errorValue;
    double eta = _chosenDirection->getErrorFunctionalScalingFactor();
    dataGrid2D chiEstimateCurrent(_grid);
    chiEstimateCurrent = _directionInput.startingChi;

    _forwardModel->calculateKappa();
    std::vector<std::complex<double>> residual = _forwardModel->calculateResidual(chiEstimateCurrent, pData);

    dataGrid2D directionCurrent(_grid);
    directionCurrent.zero();

    for(int it = 0; it < _directionInput.maxIterationsNumber; ++it)
    {
        directionCurrent = _chosenDirection->calculateDirection(chiEstimateCurrent, residual);

        _chosenStep->updateVariables(chiEstimateCurrent, directionCurrent, it);
        step = _chosenStep->calculateStepSize();

        chiEstimateCurrent = calculateNextMove(chiEstimateCurrent, directionCurrent, step);

        residual = _forwardModel->calculateResidual(chiEstimateCurrent, pData);
        errorValue = calculateErrorValue(residual, eta);
        file << std::setprecision(17) << errorValue << "," << it + 1 << std::endl;

        bar++;
    }

    return chiEstimateCurrent;
}

dataGrid2D StepAndDirectionReconstructor::calculateNextMove(const dataGrid2D &chiEstimate, const dataGrid2D &direction, double step) const
{
    dataGrid2D chiTemp = chiEstimate;
    const int nGridPoints = chiEstimate.getNumberOfGridPoints();
    std::vector<double> descentVector(nGridPoints, 0.0);
    const std::vector<double> &directionData = direction.getData();

    for(int i = 0; i < nGridPoints; ++i)
    {   // where should the -1 actually go??
        descentVector[i] = -1 * step * directionData[i];
    }
    chiTemp += descentVector;

    return chiTemp;
}

double StepAndDirectionReconstructor::calculateErrorValue(const std::vector<std::complex<double>> &residual, double eta) const
{
    return eta * _forwardModel->calculateResidualNormSq(residual);
}
