#include "gradientDescentInversion.h"
#include "gradientDescentInversionInputCardReader.h"
#include "progressBar.h"

GradientDescentInversion::GradientDescentInversion(ForwardModelInterface *forwardModel, const GradientDescentInversionInput &gdInput) :
    _forwardModel(), _gdInput(gdInput), _grid(forwardModel->getGrid()), _src(forwardModel->getSrc()), _recv(forwardModel->getRecv()),
    _freq(forwardModel->getFreq())
{
    _forwardModel = forwardModel;
}

PressureFieldSerial GradientDescentInversion::Reconstruct(const std::vector<std::complex<double>> &pData, GenericInput gInput)
{
    ProgressBar bar(_gdInput.iter);

    std::ofstream file(gInput.outputLocation + gInput.runName + "Residual.log");

    PressureFieldSerial chiEstimateCurrent(_grid);
    chiEstimateCurrent = _gdInput.x0;
    PressureFieldSerial chiEstimatePrevious(_grid);

    _forwardModel->calculateKappa();
    _forwardModel->calculateResidual(chiEstimateCurrent, pData);

    std::vector<double> dFdxCurrent(_grid.GetNumberOfGridPoints(), 0);
    std::vector<double> dFdxPrevious;

    double Fx;
    int counter = 1;
    const int nTotal = _freq.nFreq * _src.nSrc * _recv.nRecv;
    double eta = 1.0 / (normSq(pData, nTotal));
    double gamma = _gdInput.gamma0;   // First iteration
    for(int it1 = 0; it1 < _gdInput.iter; it1++)
    {
        dFdxPrevious = dFdxCurrent;
        dFdxCurrent = differential(pData, chiEstimateCurrent, _gdInput.h, eta);

        if(it1 > 0)
        {
            gamma = determineGamma(chiEstimatePrevious, chiEstimateCurrent, dFdxPrevious, dFdxCurrent);
        }

        chiEstimatePrevious = chiEstimateCurrent;
        chiEstimateCurrent = gradientDescent(chiEstimateCurrent, dFdxCurrent, gamma);
        Fx = functionF(chiEstimateCurrent, pData, eta);
        file << std::setprecision(17) << Fx << "," << counter << std::endl;

        ++counter;
        bar++;
    }

    return chiEstimateCurrent;
}

std::vector<double> GradientDescentInversion::differential(
    const std::vector<std::complex<double>> &pData, PressureFieldSerial chiEstimate, double h, double eta)
{
    const int numGridPoints = chiEstimate.GetNumberOfGridPoints();

    double fx = functionF(chiEstimate, pData, eta);

    double fxPlusH;
    PressureFieldSerial chiEstimatePlusH(chiEstimate);
    std::vector<double> dFdx(numGridPoints, 0.0);
    for(int i = 0; i < numGridPoints; ++i)
    {
        chiEstimatePlusH.addValueAtIndex(h, i);   // Add h
        fxPlusH = functionF(chiEstimatePlusH, pData, eta);
        chiEstimatePlusH.addValueAtIndex(-h, i);   // Remove h

        dFdx[i] = (fxPlusH - fx) / h;
    }
    return dFdx;
}

double GradientDescentInversion::functionF(const PressureFieldSerial chiEstimate, const std::vector<std::complex<double>> &pData, double eta)
{
    std::vector<std::complex<double>> residual = _forwardModel->calculateResidual(chiEstimate, pData);
    return eta * _forwardModel->calculateResidualNormSq(residual);
}

PressureFieldSerial GradientDescentInversion::gradientDescent(PressureFieldSerial chiEstimate, const std::vector<double> &dfdx, const double gamma)
{
    const int nGridPoints = chiEstimate.GetNumberOfGridPoints();

    std::vector<double> descentVector(nGridPoints, 0.0);
    for(int i = 0; i < nGridPoints; ++i)
    {
        descentVector[i] = -1 * gamma * dfdx[i];
    }

    chiEstimate.addData(descentVector);

    return chiEstimate;
}

double GradientDescentInversion::determineGamma(const PressureFieldSerial chiEstimatePrevious, const PressureFieldSerial chiEstimateCurrent,
    std::vector<double> dFdxPrevious, std::vector<double> dFdxCurrent)
{
    const int nGridPoints = chiEstimateCurrent.GetNumberOfGridPoints();

    const std::vector<double> &dataCurrent = chiEstimateCurrent.getData();
    const std::vector<double> &dataPrevious = chiEstimatePrevious.getData();

    std::vector<double> dx(nGridPoints, 0.0);
    for(int i = 0; i < nGridPoints; ++i)
    {
        dx[i] = dataCurrent[i] - dataPrevious[i];
    }

    std::vector<double> ddFdx(nGridPoints, 0.0);
    for(int i = 0; i < nGridPoints; ++i)
    {
        ddFdx[i] = dFdxCurrent[i] - dFdxPrevious[i];
    }

    double gammaNumerator = 0;
    for(int i = 0; i < nGridPoints; ++i)
    {
        gammaNumerator += dx[i] * ddFdx[i];
    }
    gammaNumerator = fabs(gammaNumerator);

    double gammaDenominator = 0;
    for(int i = 0; i < nGridPoints; ++i)
    {
        gammaDenominator += pow(ddFdx[i], 2);
    }

    double gamma = gammaNumerator / gammaDenominator;
    return gamma;
}
