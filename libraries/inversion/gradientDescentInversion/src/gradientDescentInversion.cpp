#include "gradientDescentInversion.h"
#include "gradientDescentInversionInputCardReader.h"


gradientDescentInversion::gradientDescentInversion(ForwardModelInterface *forwardModel, const genericInput &gInput)
    : _forwardModel(), _gdInput(), _grid(forwardModel->getGrid()), _src(forwardModel->getSrc()), _recv(forwardModel->getRecv()), _freq(forwardModel->getFreq())
{
    gradientDescentInversionInputCardReader gradientDescentInversionReader(gInput.caseFolder);
    _forwardModel = forwardModel;
    _gdInput = gradientDescentInversionReader.getInput();
}


pressureFieldSerial gradientDescentInversion::Reconstruct(const std::complex<double> *const pData, genericInput gInput)
{
    const int nTotal = _freq.nFreq * _src.nSrc * _recv.nRecv;

    double eta;
    double Fx;
    double gamma = _gdInput.gamma0;

    std::vector<double> dFdx(_grid.GetNumberOfGridPoints(), 0);
    std::vector<double> dFdxPrevious;
    std::vector<double> residuals;

    pressureFieldSerial chiEstimate(_grid);
    pressureFieldSerial chiEstimatePrevious(_grid);

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

    _forwardModel->calculateKappa();
    _forwardModel->calculateResidual(chiEstimate, pData);

    for (int it1 = 0; it1 < _gdInput.iter; it1++)
    {
        dFdxPrevious = dFdx;
        dFdx = differential(pData, chiEstimate, _gdInput.h, eta);

        if (it1 > 0)
        {
            gamma = determineGamma(dFdxPrevious, dFdx, chiEstimatePrevious, chiEstimate);
        }
        else
        {
            gamma = _gdInput.gamma0;
        }

        chiEstimatePrevious = chiEstimate;
        chiEstimate = gradientDescent(pData, chiEstimate, dFdx, gamma, eta);
        Fx = functionF(chiEstimate, pData, eta);
        file << std::setprecision(17) << Fx << "," << counter << std::endl;

        double* chiEstimateData = chiEstimate.GetDataPtr();
        
        ++counter;
    }

    return chiEstimate;
}


std::vector<double> gradientDescentInversion::differential(const std::complex<double> *const pData, pressureFieldSerial chiEstimate, double h, double eta)
{
    int numGridPoints = chiEstimate.GetNumberOfGridPoints();
    std::vector<double> dFdx(numGridPoints);

    double FxPlusH;
    double Fx = functionF(chiEstimate, pData, eta);
    pressureFieldSerial chiEstimatePlusH = chiEstimate;
    double* p_chiEstimatePlusH = chiEstimatePlusH.GetDataPtr();

    for (int i = 0; i != numGridPoints; ++i)
    {
        p_chiEstimatePlusH[i] += h;
        FxPlusH = functionF(chiEstimatePlusH, pData, eta);
        dFdx[i] = (FxPlusH - Fx) / h;
        p_chiEstimatePlusH[i] -= h;
    }
    return dFdx;
}


double gradientDescentInversion::functionF(pressureFieldSerial xi, const std::complex<double> *const pData, double eta)
{
    return eta * _forwardModel->calculateResidualNormSq(_forwardModel->calculateResidual(xi, pData));
}


pressureFieldSerial gradientDescentInversion::gradientDescent(const std::complex<double> *const pData, pressureFieldSerial x, std::vector<double> dfdx, double gamma, double eta)
{
    double* p_x = x.GetDataPtr();

    for (int i = 0; i != x.GetNumberOfGridPoints(); ++i)
    {
        p_x[i] -= gamma * dfdx[i];
    }

    return x;
}


double gradientDescentInversion::determineGamma(std::vector<double> dFdxPrevious, std::vector<double> dFdx, pressureFieldSerial xPrevious, pressureFieldSerial x)
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
