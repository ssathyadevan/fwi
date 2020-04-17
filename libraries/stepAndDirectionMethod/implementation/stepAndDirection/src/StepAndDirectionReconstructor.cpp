#include "StepAndDirectionReconstructor.h"
#include "progressBar.h"

StepAndDirectionReconstructor::StepAndDirectionReconstructor(
    StepSizeCalculator *chosenStep, DirectionCalculator *chosenDirection, forwardModelInterface *forwardModel, const DirectionInput &directionInput) :
    _chosenStep(chosenStep),
    _chosenDirection(chosenDirection), _forwardModel(), _directionInput(directionInput), _grid(forwardModel->getGrid())

{
    // should throw exceptions if things are initialized to NULL
    _forwardModel = forwardModel;
}

dataGrid2D StepAndDirectionReconstructor::reconstruct(const std::vector<std::complex<double>> &pData, genericInput gInput)
{
    progressBar bar(_directionInput._maxIterationsNumber);

    std::ofstream file(gInput.outputLocation + gInput.runName + "Residual.log");

    double step = 0.0;

    //    In conjugateGradient all of these variables are initialized at every cycle of the
    //    loop, because they are also used in that inner loop, that we do not have here.

    double fx;
    int counter = 1;
    double eta = 1.0 / (normSq(pData));
    dataGrid2D chiEstimateCurrent(_grid);
    chiEstimateCurrent = _directionInput._startingChi;
    // dataGrid2D chiEstimatePrevious(_grid);

    _forwardModel->calculateKappa();

    dataGrid2D directionCurrent(_grid);
    directionCurrent.zero();

    // dataGrid2D directionPrevious(_grid);
    // directionPrevious.zero();

    for(int it = 0; it < _directionInput._maxIterationsNumber; it++)
    {
        //    directionPrevious = directionCurrent;
        directionCurrent = _chosenDirection->calculateDirection(chiEstimateCurrent, pData);

        _chosenStep->updateVariables(chiEstimateCurrent, directionCurrent);
        if(it > 0)
        {
            // direction MIGHT NOT BE dFdx !!!
            step = _chosenStep->calculateStepSize();   //(chiEstimatePrevious, chiEstimateCurrent, directionPrevious, directionCurrent);
            // need to add inputs to all types of StepSize
        }

        //    chiEstimatePrevious = chiEstimateCurrent;
        chiEstimateCurrent = calculateNextMove(chiEstimateCurrent, directionCurrent, step);

        fx = functionF(chiEstimateCurrent, pData, eta);
        file << std::setprecision(17) << fx << "," << counter << std::endl;

        ++counter;   // this is always equal to it+1, can replace?
        bar++;
    }

    return chiEstimateCurrent;
}

dataGrid2D StepAndDirectionReconstructor::calculateNextMove(const dataGrid2D &chiEstimate, const dataGrid2D &direction, double step)
{
    dataGrid2D chiTemp = chiEstimate;
    const int nGridPoints = chiEstimate.getNumberOfGridPoints();
    std::vector<double> descentVector(nGridPoints, 0.0);
    std::vector<double> directionData = direction.getData();

    for(int i = 0; i < nGridPoints; ++i)
    {
        descentVector[i] = -1 * step * directionData[i];
    }
    chiTemp += descentVector;

    return chiTemp;
}

double StepAndDirectionReconstructor::functionF(const dataGrid2D chiEstimate, const std::vector<std::complex<double>> &pData, double eta)
{
    std::vector<std::complex<double>> residual = _forwardModel->calculateResidual(chiEstimate, pData);
    return eta * _forwardModel->calculateResidualNormSq(residual);
}

double StepAndDirectionReconstructor::normSq(const std::vector<std::complex<double>> &pData)
{
    double total = 0.0;
    int nMax = pData.size();

    for(int i = 0; i < nMax; ++i)
    {
        total += std::norm(pData[i]);
    }
    return total;
}
