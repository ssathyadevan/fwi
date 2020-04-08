#include "stepAndDirectionInterface.h"
#include "progressBar.h"

StepAndDirectionInterface::StepAndDirectionInterface(const ChosenStep step, const ChosenDirection direction) :
    _chosenStep(step), _chosenDirection(direction), _forwardModel(), _input_grid(forwardModel->getGrid()), _src(forwardModel->getSrc()),
    _recv(forwardModel->getRecv()), _freq(forwardModel->getFreq())
{
    _forwardModel = forwardModel;
}
dataGrid2D StepAndDirectionInterface::reconstruct(const std::vector<std::complex<double>> &pData, genericInput gInput)
{
    progressBar bar(_directionInput.iter);

    std::ofstream file(gInput.outputLocation + gInput.runName + "Residual.log");

    dataGrid2D chiEstimateCurrent(_grid);
    chiEstimateCurrent = _directionInput.x0;
    dataGrid2D chiEstimatePrevious(_grid);

    _forwardModel->calculateKappa();
    _forwardModel->calculateResidual(chiEstimateCurrent, pData);

    std::vector<double> directionCurrent(_grid.getNumberOfGridPoints(), 0);
    std::vector<double> directionPrevious;

    double fx;
    int counter = 1;
    const int nTotal = _freq.nFreq * _src.nSrc * _recv.nRecv;
    double eta = 1.0 / (normSq(pData, nTotal));
    double step = _directionInput.gamma0;   // this line belongs only to gradient descent

    for(int it1 = 0; it1 < _directionInput.iter; it1++)
    {
        // these are used for the gamma computation
        directionPrevious = directionCurrent;
        directionCurrent = _chosenDirection->calculateDirection(chiEstimateCurrent, directionCurrent, step);   // MISSING
        //        directionCurrent = differential(pData, chiEstimateCurrent, _gdInput.h, eta); //this the the computation of the direction

        if(it1 > 0)
        {
            // HERE compute Step using _chosenStep
            // direction... MIGHT NOT BE dFdx... !!!
            step = _chosenStep->calculateStep(chiEstimatePrevious, chiEstimateCurrent, directionPrevious, directionCurrent);
            // gamma = determineGamma(chiEstimatePrevious, chiEstimateCurrent, dFdxPrevious, dFdxCurrent);
        }

        chiEstimatePrevious = chiEstimateCurrent;
        // this function ONLY multiplies step by direction
        chiEstimateCurrent = calculateNextMove(chiEstimateCurrent, directionCurrent, gamma);

        fx = functionF(chiEstimateCurrent, pData, eta);
        file << std::setprecision(17) << fx << "," << counter << std::endl;

        ++counter;
        bar++;
    }

    return chiEstimateCurrent;
}

dataGrid2D StepAndDirectionInterface::calculateNextMove(dataGrid2D chiEstimate, const std::vector<double> &direction, const double step)
{
    const int nGridPoints = chiEstimate.getNumberOfGridPoints();

    std::vector<double> descentVector(nGridPoints, 0.0);
    for(int i = 0; i < nGridPoints; ++i)
    {
        descentVector[i] = -1 * step * direction[i];
    }
    chiEstimate += descentVector;

    return chiEstimate;
}

double StepAndDirectionInterface::functionF(const dataGrid2D chiEstimate, const std::vector<std::complex<double>> &pData, double eta)
{
    std::vector<std::complex<double>> residual = _forwardModel->calculateResidual(chiEstimate, pData);
    return eta * _forwardModel->calculateResidualNormSq(residual);
}
