#include "OpenMPgradientDescentInversion.h"
#include "gradientDescentInversionInputCardReader.h"
#include "progressBar.h"
#include <iostream>
#include <omp.h>
#include <sstream>

OpenMPGradientDescentInversion::OpenMPGradientDescentInversion(const genericInput &gInput, ForwardModelContainer &forwardmodels) :
    _forwardModelsParallel(forwardmodels), _gdInput(), _grid(forwardmodels.getGrid()), _src(forwardmodels.getSources()), _recv(forwardmodels.getReceivers()),
    _freq(forwardmodels.getFrequencies())
{
    gradientDescentInversionInputCardReader GradientDescentInversionReader(gInput.caseFolder);
    _gdInput = GradientDescentInversionReader.getInput();
}

pressureFieldSerial OpenMPGradientDescentInversion::reconstruct(const std::vector<std::complex<double>> &pData, genericInput gInput)
{
    const int nTotal = _freq.nFreq * _src.nSrc * _recv.nRecv;
    L_(linfo) << "Parallelization of gradient descent.";
    progressBar bar(_gdInput.iter);

    double eta;
    double Fx;
    double gamma = _gdInput.gamma0;

    std::vector<double> dFdx(_grid.getNumberOfGridPoints(), 0.0);

    std::vector<double> dFdxPrevious;

    pressureFieldSerial chiEstimate(_grid);
    pressureFieldSerial chiEstimatePrevious(_grid);

    chiEstimate.zero();
    chiEstimatePrevious.zero();
    int counter = 1;

    eta = 1.0 / (normSq(pData, nTotal));

    double *p_chiEstimate = chiEstimate.getDataPtr();
    for(int i = 0; i != chiEstimate.getNumberOfGridPoints(); ++i)
    {
        p_chiEstimate[i] = _gdInput.x0;
    }

    std::ofstream file(gInput.outputLocation + gInput.runName + "Residual.log");

    _forwardModelsParallel.calculateKappaParallel();

    for(int it1 = 0; it1 < _gdInput.iter; it1++)
    {
        dFdxPrevious = dFdx;
        dFdx = differentialParallel(pData, chiEstimate, _gdInput.h, eta);

        if(it1 > 0) { gamma = determineGamma(dFdxPrevious, dFdx, chiEstimatePrevious, chiEstimate); }
        else
        {
            gamma = _gdInput.gamma0;
        }

        chiEstimatePrevious = chiEstimate;
        chiEstimate = gradientDescent(chiEstimate, dFdx, gamma);
        Fx = functionFParallel(chiEstimate, pData, eta);
        file << std::setprecision(17) << Fx << "," << counter << std::endl;

        ++counter;
        bar++;
    }

    return chiEstimate;
}

double OpenMPGradientDescentInversion::functionFParallel(const pressureFieldSerial &xi, const std::vector<std::complex<double>> &pData, double eta)
{
    return eta * _forwardModelsParallel.calculateResidualNormSqParallel(_forwardModelsParallel.calculateResidualParallel(xi, pData));
}

pressureFieldSerial OpenMPGradientDescentInversion::gradientDescent(pressureFieldSerial x, std::vector<double> dfdx, double gamma)
{
    double *p_x = x.getDataPtr();

    for(int i = 0; i != x.getNumberOfGridPoints(); ++i)
    {
        p_x[i] -= gamma * dfdx[i];
    }

    return x;
}

std::vector<double> OpenMPGradientDescentInversion::differentialParallel(
    const std::vector<std::complex<double>> &pData, pressureFieldSerial chiEstimate, double h, double eta)
{
    int numGridPoints = chiEstimate.getNumberOfGridPoints();
    double FxPlusH;
    std::vector<double> dFdx(numGridPoints);
    double Fx = functionFParallel(chiEstimate, pData, eta);
    pressureFieldSerial chiEstimatePlusH = chiEstimate;

    for(int i = 0; i != numGridPoints; ++i)
    {
        chiEstimatePlusH.PlusElement(i, h);
        FxPlusH = functionFParallel(chiEstimatePlusH, pData, eta);
        dFdx[i] = (FxPlusH - Fx) / h;
        chiEstimatePlusH.PlusElement(i, -h);
    }
    return dFdx;
}

double OpenMPGradientDescentInversion::determineGamma(
    std::vector<double> dFdxPrevious, std::vector<double> dFdx, pressureFieldSerial xPrevious, pressureFieldSerial x)
{
    double *p_x = x.getDataPtr();
    double *p_xPrevious = xPrevious.getDataPtr();

    int nGridPoints = x.getNumberOfGridPoints();

    std::vector<double> dx(nGridPoints);
    for(int i = 0; i != nGridPoints; ++i)
    {
        dx[i] = p_x[i] - p_xPrevious[i];
    }

    std::vector<double> ddFdx(nGridPoints);
    for(int i = 0; i != nGridPoints; ++i)
    {
        ddFdx[i] = dFdx[i] - dFdxPrevious[i];
    }

    double gammaNumerator = 0;
    for(int i = 0; i != nGridPoints; ++i)
    {
        gammaNumerator += dx[i] * ddFdx[i];
    }
    gammaNumerator = fabs(gammaNumerator);

    double gammaDenominator = 0;
    for(int i = 0; i != nGridPoints; ++i)
    {
        gammaDenominator += pow(ddFdx[i], 2);
    }

    double gamma = gammaNumerator / gammaDenominator;

    return gamma;
}
