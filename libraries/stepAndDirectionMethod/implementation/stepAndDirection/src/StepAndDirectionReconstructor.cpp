#include "StepAndDirectionReconstructor.h"
#include "progressBar.h"

StepAndDirectionReconstructor::StepAndDirectionReconstructor(StepSizeCalculator *desiredStep, DirectionCalculator *desiredDirection,
    forwardModelInterface *forwardModel, const ReconstructorParameters &directionInput) :
    _desiredStep(desiredStep),
    _desiredDirection(desiredDirection), _forwardModel(forwardModel), _directionInput(directionInput), _grid(forwardModel->getGrid())
{
}

dataGrid2D StepAndDirectionReconstructor::reconstruct(const std::vector<std::complex<double>> &pData, genericInput gInput)
{
    progressBar bar(_directionInput.maxIterationsNumber);

    std::ofstream file(gInput.outputLocation + gInput.runName + "Residual.log");

    double step = 0.0;

    double eta = _desiredDirection->getErrorFunctionalScalingFactor();

    dataGrid2D chiEstimateCurrent(_grid);
    chiEstimateCurrent = _directionInput.startingChi;

    std::vector<std::complex<double>> residualVector = _forwardModel->calculateResidual(chiEstimateCurrent, pData);
    double residualValue = calculateResidualNorm(residualVector, eta);

    _forwardModel->calculateKappa();

    dataGrid2D const *directionCurrent;

    std::vector<std::complex<double>> kappaTimesDirection;

    for(int it = 0; it < _directionInput.maxIterationsNumber; ++it)
    {
        directionCurrent = &_desiredDirection->calculateDirection(chiEstimateCurrent, residualVector);

        // here we compute kappaTimesDirection, which is used only in ConjugateGradientStepSize.
        _forwardModel->mapDomainToSignal(*directionCurrent, kappaTimesDirection);

        _desiredStep->updateVariables(
            chiEstimateCurrent, *directionCurrent, it, kappaTimesDirection, residualVector);   // update all StepSizeCalculator with last two items

        step = _desiredStep->calculateStepSize();

        chiEstimateCurrent = calculateNextMove(chiEstimateCurrent, *directionCurrent, step);

        residualVector = _forwardModel->calculateResidual(chiEstimateCurrent, pData);
        residualValue = calculateResidualNorm(residualVector, eta);
        file << std::setprecision(17) << residualValue << "," << it + 1 << std::endl;

        if(residualValue < _directionInput.tolerance)
        {
            file << "Reconstruction terminated at iteration " << it + 1 << " because error is smaller than input tolerance " << std::setprecision(17)
                 << _directionInput.tolerance << std::endl;
            break;
        }

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
    {   // make sure the DirectionCalculators have the -1 integrated
        descentVector[i] = step * directionData[i];
    }
    chiTemp += descentVector;

    return chiTemp;
}

double StepAndDirectionReconstructor::calculateResidualNorm(const std::vector<std::complex<double>> &residualVector, double eta) const
{
    return eta * _forwardModel->calculateResidualNormSq(residualVector);
}
