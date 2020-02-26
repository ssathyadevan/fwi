#include "OpenMPIgradientDescentInversion.h"
#include "gradientDescentInversionInputCardReader.h"
#include "progressBar.h"
#include <omp.h>

OpenMPIGradientDescentInversion::OpenMPIGradientDescentInversion(ForwardModelInterface *forwardModel, const GenericInput &gInput)
    : _forwardModel(), _gdInput(), _grid(forwardModel->getGrid()), _src(forwardModel->getSrc()), _recv(forwardModel->getRecv()), _freq(forwardModel->getFreq())
{
    GradientDescentInversionInputCardReader GradientDescentInversionReader(gInput.caseFolder);
    _forwardModel = forwardModel;
    _gdInput = GradientDescentInversionReader.getInput();
}


PressureFieldSerial OpenMPIGradientDescentInversion::Reconstruct(const std::vector<std::complex<double>> &pData, GenericInput gInput)
{
    const int nTotal = _freq.nFreq * _src.nSrc * _recv.nRecv;
    std::cout << "Parallelization of Gradient descent with max" << omp_get_max_threads() << " threads." << std::endl;
    ProgressBar bar(_gdInput.iter);

    double eta;
    double Fx;
    double gamma = _gdInput.gamma0;

    std::vector<double> dFdx(_grid.GetNumberOfGridPoints(), 0);
    std::vector<double> dFdxPrevious;
    std::vector<double> residuals;

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
        chiEstimate = gradientDescent(chiEstimate, dFdx, gamma);
        Fx = functionF(chiEstimate, pData, eta);
        file << std::setprecision(17) << Fx << "," << counter << std::endl;
        
        ++counter;
        bar++;
    }

    return chiEstimate;
}

std::vector<double> OpenMPIGradientDescentInversion::differential(const std::vector<std::complex<double>> &pData, PressureFieldSerial chiEstimate, double h, double eta)
{
    int numGridPoints = chiEstimate.GetNumberOfGridPoints();
    std::vector<double> dFdx(numGridPoints);
    //std::cout << "\n Start parallelization of Differential with " << omp_get_num_threads() << " threads." << std::endl;
    #pragma omp parallel shared(dFdx) shared(pData) shared(chiEstimate) shared(h) shared(eta)
    {
        if (omp_get_thread_num() == 0)
        {
            std::cout << "\n Start parallelization of Differential with " << omp_get_num_threads() << " threads." << std::endl;
        }
        const std::vector<std::complex<double>> mypData = pData;
        PressureFieldSerial mychi = chiEstimate;
        unsigned int blocksize = dFdx.size() / omp_get_num_threads();
        unsigned int offset = blocksize * omp_get_thread_num();
        if (offset + blocksize > dFdx.size() )
        {
            blocksize = dFdx.size() - offset;
        }
        std::vector<double> my_dFdx(blocksize);
        differential_parallel(mypData, mychi, h, eta, blocksize, offset, my_dFdx);
        std::copy(my_dFdx.begin(), my_dFdx.end(), dFdx.begin() + offset);
    }
    std::cout << "Finish parallelization \n" << std::endl;
    return dFdx;
}


double OpenMPIGradientDescentInversion::functionF(PressureFieldSerial xi, const std::vector<std::complex<double>> &pData, double eta)
{
    return eta * _forwardModel->calculateResidualNormSq(_forwardModel->calculateResidual(xi, pData));
}


PressureFieldSerial OpenMPIGradientDescentInversion::gradientDescent(PressureFieldSerial x, std::vector<double> dfdx, double gamma)
{
    double* p_x = x.GetDataPtr();

    for (int i = 0; i != x.GetNumberOfGridPoints(); ++i)
    {
        p_x[i] -= gamma * dfdx[i];
    }

    return x;
}

void OpenMPIGradientDescentInversion::differential_parallel(const std::vector<std::complex<double>> &pData, PressureFieldSerial chiEstimate, double h, double eta, int blocksize, int offset, std::vector<double> &my_dFdx)
{
    double FxPlusH;
    double Fx = functionF(chiEstimate, pData, eta);
    PressureFieldSerial chiEstimatePlusH = chiEstimate;
    double* p_chiEstimatePlusH = chiEstimatePlusH.GetDataPtr();

    for (int i = offset; i != offset + blocksize; ++i)
    {
        p_chiEstimatePlusH[i] += h;
        FxPlusH = functionF(chiEstimatePlusH, pData, eta);
        my_dFdx[i - offset] = (FxPlusH - Fx) / h;
        p_chiEstimatePlusH[i] -= h;
    }
}


double OpenMPIGradientDescentInversion::determineGamma(std::vector<double> dFdxPrevious, std::vector<double> dFdx, PressureFieldSerial xPrevious, PressureFieldSerial x)
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
