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
    double gamma;
    double residual;

    std::vector<double> dfdx(_grid.GetNumberOfGridPoints(), 0);
    std::vector<double> dfdxPrevious;
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
        p_chiEstimate[i] = _gdInput.dx;
    }

    std::ofstream file(gInput.outputLocation + gInput.runName + "Residual.log");

    _forwardModel->calculateKappa();
    _forwardModel->calculateResidual(chiEstimate, pData);

    for (int it1 = 0; it1 < _gdInput.iter; it1++)
    {
        dfdxPrevious = dfdx;
        dfdx = differential(pData, chiEstimate, _gdInput.gamma, eta);

        gamma = determineGamma(dfdxPrevious, dfdx, chiEstimatePrevious, chiEstimate);

        chiEstimatePrevious = chiEstimate;
        chiEstimate = gradientDescent(pData, chiEstimate, dfdx, gamma, eta);
        Fx = functionF(chiEstimate, pData, eta);
        file << std::setprecision(17) << residual << "," << counter << std::endl;

        ++counter;
    }

    return chiEstimate;
}


std::vector<double> gradientDescentInversion::differential(const std::complex<double> *const pData, pressureFieldSerial xi, double dxi, double eta)
{

    std::vector<double> dFdx(xi.GetNumberOfGridPoints());
    double Fx = functionF(xi, pData, eta);
    pressureFieldSerial xidxi = xi;
    std::cout << "The residual is :" << Fx << std::endl;
    for (int i = 0; i < xi.GetNumberOfGridPoints(); i++)
    {

        xidxi.GetDataPtr()[i] += dxi;

        double Fxdx = functionF(xidxi, pData, eta);
        dFdx[i] = (Fxdx - Fx) / dxi;
        xidxi.GetDataPtr()[i] = xi.GetDataPtr()[i];
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

    for (int j = 0; j != x.GetNumberOfGridPoints(); j++)
    {
        p_x[j] -= gamma * dfdx[j];
    }

    return x;
}


double gradientDescentInversion::determineGamma(std::vector<double> dfdxPrevious, std::vector<double> dfdx, pressureFieldSerial xPrevious, pressureFieldSerial x)
{
    double* p_x = x.GetDataPtr();
    double* p_xPrevious = xPrevious.GetDataPtr();

    int nGridPoints = x.GetNumberOfGridPoints();
    
    std::ofstream xFile("/home/chris/Documents/parallelized full waveform inversion/FWIInstall/some_name/output/x_values.log");
    std::ofstream xPreviousFile("/home/chris/Documents/parallelized full waveform inversion/FWIInstall/some_name/output/x_prev_values.log"); 

    std::vector<double> dx(nGridPoints);
    for (int i = 0; i != nGridPoints; ++i) 
    {
        dx[i] = p_x[i] - p_xPrevious[i];
    }

    std::vector<double> ddfdx(nGridPoints);
    for (int i = 0; i != nGridPoints; ++i) 
    {
        ddfdx[i] = dfdx[i] - dfdxPrevious[i];
    }

    double gammaNumerator = 0;
    for  (int i = 0; i != nGridPoints; ++i) 
    {
        gammaNumerator += dx[i]*ddfdx[i];
    }
    gammaNumerator = fabs(gammaNumerator);

    double gammaDenominator = 0;
    for  (int i = 0; i != nGridPoints; ++i) 
    {
        gammaDenominator += pow(ddfdx[i], 2);
    }
    gammaDenominator = pow(gammaDenominator, 0.5);

    double gamma = gammaNumerator / gammaDenominator;

    return gamma;
}
