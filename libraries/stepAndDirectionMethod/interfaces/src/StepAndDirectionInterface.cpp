#include "StepAndDirectionInterface.h"
#include "progressBar.h"

StepAndDirectionInterface::StepAndDirectionInterface(
    StepSizeCalculator *chosenStep, DirectionCalculator *chosenDirection, forwardModelInterface *forwardModel, const DirectionInput &directionInput) :
    _chosenStep(chosenStep),
    _chosenDirection(chosenDirection), _forwardModel(), _directionInput(directionInput), _grid(forwardModel->getGrid())

{
    // should throw exceptions if things are initialized to NULL
    _forwardModel = forwardModel;
}

dataGrid2D StepAndDirectionInterface::reconstruct(const std::vector<std::complex<double>> &pData, genericInput gInput)
{
    progressBar bar(_directionInput._maxIterationsNumber);

    std::ofstream file(gInput.outputLocation + gInput.runName + "Residual.log");

    double step = 0.0;

    // in conjugateGradient all of these variables are initialized at every cycle of the loop, because they
    // are also used in that inner loop, that we do not have here.

    double fx;
    int counter = 1;
    double eta = 1.0 / (normSq(pData));
    dataGrid2D chiEstimateCurrent(_grid);
    chiEstimateCurrent = _directionInput._startingChi;
    dataGrid2D chiEstimatePrevious(_grid);

    _forwardModel->calculateKappa();
    //    complexDataGrid2D residual = _forwardModel->calculateResidual(chiEstimateCurrent, pData);
    // std::vector<std::complex<double>> residual = _forwardModel->calculateResidual(chiEstimateCurrent, pData);

    dataGrid2D directionCurrent(_grid);
    directionCurrent.zero();

    dataGrid2D directionPrevious(_grid);
    directionPrevious.zero();

    for(int it = 0; it < _directionInput._maxIterationsNumber; it++)
    {
        directionPrevious = directionCurrent;
        directionCurrent = _chosenDirection->calculateDirection(chiEstimateCurrent, pData);   // the second input should be pData rather than directionCurrent

        if(it > 0)
        {
            // HERE compute Step using _chosenStep
            // direction MIGHT NOT BE dFdx !!!
            step = _chosenStep->calculateStepSize();   //(chiEstimatePrevious, chiEstimateCurrent, directionPrevious, directionCurrent);
            // need to add inputs to all types of StepSize
        }

        chiEstimatePrevious = chiEstimateCurrent;
        chiEstimateCurrent = calculateNextMove(chiEstimateCurrent, directionCurrent, step);

        fx = functionF(chiEstimateCurrent, pData, eta);
        file << std::setprecision(17) << fx << "," << counter << std::endl;

        ++counter;   // this is always equal to it+1
        bar++;
    }

    return chiEstimateCurrent;
}

dataGrid2D StepAndDirectionInterface::calculateNextMove(dataGrid2D chiEstimate, const dataGrid2D &direction, double step)
{
    const int nGridPoints = chiEstimate.getNumberOfGridPoints();
    std::vector<double> descentVector(nGridPoints, 0.0);
    std::vector<double> directionData = direction.getData();

    for(int i = 0; i < nGridPoints; ++i)
    {
        descentVector[i] = -1 * step * directionData[i];
    }
    chiEstimate += descentVector;

    return chiEstimate;
}

double StepAndDirectionInterface::functionF(const dataGrid2D chiEstimate, const std::vector<std::complex<double>> &pData, double eta)
{
    std::vector<std::complex<double>> residual = _forwardModel->calculateResidual(chiEstimate, pData);
    //    complexDataGrid2D residual = _forwardModel->calculateResidual(chiEstimate, pData);
    return eta * _forwardModel->calculateResidualNormSq(residual);
}

double StepAndDirectionInterface::normSq(const std::vector<std::complex<double>> &pData)
{
    double total = 0.0;
    int nMax = pData.size();

    for(int i = 0; i < nMax; ++i)
    {
        total += std::norm(pData[i]);
    }
    return total;
}
