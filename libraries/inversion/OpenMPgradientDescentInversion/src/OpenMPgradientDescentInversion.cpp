#include "OpenMPgradientDescentInversion.h"
#include "gradientDescentInversionInputCardReader.h"
#include "progressBar.h"
#include <omp.h>
#include <sstream>
#include <iostream>

OpenMPGradientDescentInversion::OpenMPGradientDescentInversion(const GenericInput &gInput, ForwardModelContainer& forwardmodels)
    : _forwardModelsParallel(forwardmodels), _gdInput(), _grid(forwardmodels.getGrid()), _src(forwardmodels.getSources()), _recv(forwardmodels.getReceivers()), _freq(forwardmodels.getFrequencies())
{
    GradientDescentInversionInputCardReader GradientDescentInversionReader(gInput.caseFolder);
    _gdInput = GradientDescentInversionReader.getInput();
}


PressureFieldSerial OpenMPGradientDescentInversion::Reconstruct(const std::vector<std::complex<double>> &pData, GenericInput gInput)
{
    const int nTotal = _freq.nFreq * _src.nSrc * _recv.nRecv;
    L_(linfo)<< "Parallelization of Gradient descent with " << omp_get_max_threads() << " threads.";
    ProgressBar bar(_gdInput.iter);

    double eta;
    double Fx;
    double gamma = _gdInput.gamma0;

    std::vector<double> dFdx(_grid.GetNumberOfGridPoints(), 0.0);

    std::vector<double> dFdxPrevious;

    PressureFieldSerial chiEstimate(_grid);
    PressureFieldSerial chiEstimatePrevious(_grid);

    chiEstimate.Zero();
    chiEstimatePrevious.Zero();
    int counter = 1;

    eta = 1.0 / (normSq(pData, nTotal));

    double* p_chiEstimate = chiEstimate.GetDataPtr();
    for (int i = 0; i != chiEstimate.GetNumberOfGridPoints(); ++i) 
    {
        p_chiEstimate[i] = _gdInput.x0;
    }

    std::ofstream file(gInput.outputLocation + gInput.runName + "Residual.log");

    _forwardModelsParallel.calculateKappaParallel();

    for (int it1 = 0; it1 < _gdInput.iter; it1++)
    {
        dFdxPrevious = dFdx;
        dFdx = differential_parallel(pData, chiEstimate, _gdInput.h, eta);

        if (it1 > 0)
        {
            gamma = determineGamma(dFdxPrevious, dFdx, chiEstimatePrevious, chiEstimate);
        }
        else
        {
            gamma = _gdInput.gamma0;
        }

        chiEstimatePrevious = chiEstimate;
        chiEstimate = gradientDescent(chiEstimate, dFdx, gamma);
        Fx = functionF_parallel(chiEstimate, pData, eta);
        file << std::setprecision(17) << Fx << "," << counter << std::endl;
        
        ++counter;
        bar++;
    }

    return chiEstimate;
}



double OpenMPGradientDescentInversion::functionF_parallel(const PressureFieldSerial &xi, const std::vector<std::complex<double> > &pData, double eta)
{
    return eta * _forwardModelsParallel.calculateResidualNormSqParallel(_forwardModelsParallel.calculateResidualParallel(xi,pData));
}


PressureFieldSerial OpenMPGradientDescentInversion::gradientDescent(PressureFieldSerial x, std::vector<double> dfdx, double gamma)
{
    double* p_x = x.GetDataPtr();

    for (int i = 0; i != x.GetNumberOfGridPoints(); ++i)
    {
        p_x[i] -= gamma * dfdx[i];
    }

    return x;
}

std::vector<double> OpenMPGradientDescentInversion::differential_parallel(const std::vector<std::complex<double>> &pData, PressureFieldSerial chiEstimate, double h, double eta)
{

    int numGridPoints = chiEstimate.GetNumberOfGridPoints();
    double FxPlusH;
    std::vector<double> dFdx(numGridPoints);
    double Fx = functionF_parallel(chiEstimate, pData, eta);
    PressureFieldSerial chiEstimatePlusH = chiEstimate;

    for (int i = 0; i != numGridPoints; ++i)
    {
        chiEstimatePlusH.PlusElement(i,h);
        FxPlusH = functionF_parallel(chiEstimatePlusH, pData, eta);
        dFdx[i] = (FxPlusH - Fx) / h;
        chiEstimatePlusH.PlusElement(i,-h);
    }
    return dFdx;
}


double OpenMPGradientDescentInversion::determineGamma(std::vector<double> dFdxPrevious, std::vector<double> dFdx, PressureFieldSerial xPrevious, PressureFieldSerial x)
{
    double* p_x = x.GetDataPtr();
    double* p_xPrevious = xPrevious.GetDataPtr();

    int nGridPoints = x.GetNumberOfGridPoints();

    std::vector<double> dx(nGridPoints);
    for (int i = 0; i != nGridPoints; ++i) 
    {
        dx[i] = p_x[i] - p_xPrevious[i];
    }

    std::vector<double> ddFdx(nGridPoints);
    for (int i = 0; i != nGridPoints; ++i) 
    {
        ddFdx[i] = dFdx[i] - dFdxPrevious[i];
    }

    double gammaNumerator = 0;
    for  (int i = 0; i != nGridPoints; ++i) 
    {
        gammaNumerator += dx[i]*ddFdx[i];
    }
    gammaNumerator = fabs(gammaNumerator);

    double gammaDenominator = 0;
    for  (int i = 0; i != nGridPoints; ++i) 
    {
        gammaDenominator += pow(ddFdx[i], 2);
    }

    double gamma = gammaNumerator / gammaDenominator;

    return gamma;
}
