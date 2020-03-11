#include "OpenMPgradientDescentInversion.h"
#include "gradientDescentInversionInputCardReader.h"
#include "progressBar.h"
#include <omp.h>
#include <sstream>
#include <iostream>

OpenMPGradientDescentInversion::OpenMPGradientDescentInversion(ForwardModelInterface *forwardModel, const GenericInput &gInput)
    : _forwardModel(), _gdInput(), _grid(forwardModel->getGrid()), _src(forwardModel->getSrc()), _recv(forwardModel->getRecv()), _freq(forwardModel->getFreq())
{
    GradientDescentInversionInputCardReader GradientDescentInversionReader(gInput.caseFolder);
    _forwardModel = forwardModel;
    _gdInput = GradientDescentInversionReader.getInput();
}


PressureFieldSerial OpenMPGradientDescentInversion::Reconstruct(const std::vector<std::complex<double>> &pData, GenericInput gInput)
{
    const int nTotal = _freq.nFreq * _src.nSrc * _recv.nRecv;
    std::cout << "Parallelization of Gradient descent with max" << omp_get_max_threads() << " threads." << std::endl;
    ProgressBar bar(_gdInput.iter);

    double eta;
    double Fx;
    double gamma = _gdInput.gamma0;

    std::vector<double> dFdx(_grid.GetNumberOfGridPoints(), 0);
    std::vector<double> dFdxSerial(_grid.GetNumberOfGridPoints(), 0);

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
        dFdxSerial = differentialSerial(pData, chiEstimate, _gdInput.h, eta);
        int numDifferences = 0;

        for (int i = 0; i < static_cast<int>(dFdx.size()); i++)
        {
            std::cout << "dFdx serial = " << dFdxSerial[i] << "| dFdx parallel = " << dFdx[i] << std::endl;



            if ((dFdx[i] - dFdxSerial[i]) < 0.0001)
            {
                numDifferences++;
            }
        }

        if (numDifferences > 0)
        {
            //std::cout << "There are " << numDifferences << " differences between the serial and parallel methods" << std::endl;
        }

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

std::vector<double> OpenMPGradientDescentInversion::differential(const std::vector<std::complex<double>>& pData, const PressureFieldSerial& chiEstimate, const double h, const double eta)
{
    int numGridPoints = chiEstimate.GetNumberOfGridPoints();
    std::vector<double> dFdx(numGridPoints, 0);

    const double Fx = functionF(chiEstimate, pData, eta);

    int blockSize;

#pragma omp parallel
    {
        blockSize = numGridPoints / omp_get_num_threads();
        int my_offset = omp_get_thread_num() * blockSize;
        //std::cout << "I am Thread " << omp_get_thread_num() << " and my offset is " << my_offset << std::endl;
        std::vector<double> my_dFdx(blockSize);
        for (int i = my_offset; i < my_offset + blockSize; ++i)
        {
            PressureFieldSerial chiEstimatePlusH = chiEstimate;
            chiEstimatePlusH.PlusElement(i, h);
            const double FxPlusH = functionF(chiEstimatePlusH, pData, eta);
            my_dFdx[i - my_offset] = (FxPlusH - Fx) / h;
        }
        std::copy(my_dFdx.begin(), my_dFdx.end(), dFdx.begin() + my_offset);
    }
    return dFdx;
}

std::vector<double> OpenMPGradientDescentInversion::differentialSerial(const std::vector<std::complex<double>> &pData, PressureFieldSerial chiEstimate, double h, double eta)
{
    int numGridPoints = chiEstimate.GetNumberOfGridPoints();
    std::vector<double> dFdx(numGridPoints);

    double FxPlusH;
    double Fx = functionF(chiEstimate, pData, eta);
    PressureFieldSerial chiEstimatePlusH = chiEstimate;
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



double OpenMPGradientDescentInversion::functionF(const PressureFieldSerial &xi, const std::vector<std::complex<double>> &pData, double eta)
{
    return eta * _forwardModel->calculateResidualNormSq(_forwardModel->calculateResidual(xi, pData));
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

std::vector<double> OpenMPGradientDescentInversion::differential_parallel(const std::vector<std::complex<double>> &pData, PressureFieldSerial chiEstimate, double h, double eta, unsigned int blocksize, unsigned int offset)
{
    double FxPlusH;
    double Fx = functionF(chiEstimate, pData, eta);
    PressureFieldSerial chiEstimatePlusH = chiEstimate;
    std::vector<double> my_dFdx(blocksize, 0.0);
    //double* p_chiEstimatePlusH = chiEstimatePlusH.GetDataPtr();
    std::vector<double> expectedDFdx= {-1.44655, -2.65177, -4.61663, -5.35039, -2.94658, -1.57442,1.35354,2.75141,3.31655,3.44341,2.76171,1.45595,0.495105,0.514024, 0.249245, 0.204169,0.543168,0.472388};

    for (unsigned int i = offset; i != offset + blocksize; ++i)
    {
        chiEstimatePlusH.PlusElement(i,h);
        FxPlusH = functionF(chiEstimatePlusH, pData, eta);
        my_dFdx[i - offset] = (FxPlusH - Fx) / h;
        chiEstimatePlusH.PlusElement(i,-h);
    }
    return my_dFdx;
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
