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

std::vector<double> OpenMPGradientDescentInversion::differential(const std::vector<std::complex<double>>& pData, const PressureFieldSerial chiEstimate, const double h, const double eta)
{
    const int numGridPoints = chiEstimate.GetNumberOfGridPoints();
    std::vector<double> dFdx(numGridPoints, 0.0);

    const double Fx = functionF(chiEstimate, pData, eta);
    std::vector<omp_lock_t> locks(numGridPoints);

    for( omp_lock_t lock : locks)
    {
        omp_init_lock(&lock);
    }

    //omp_set_num_threads(1);
#pragma omp parallel default(none) firstprivate(pData) shared(dFdx) shared(locks) //shared(stream) private(mystream)
    {
        double FxPlusH = 0.0;
        PressureFieldSerial chiEstimatePlusH = chiEstimate;

#pragma omp for
        for (int i = 0; i < numGridPoints; i++)
        {
            omp_set_lock(&(locks[i]));
            chiEstimatePlusH.PlusElement(i,h);
            FxPlusH= functionF(chiEstimatePlusH, pData, eta);

            dFdx[i] = (FxPlusH- Fx) / h;
            chiEstimatePlusH.PlusElement(i,-h);
            FxPlusH= 0.0;
            omp_unset_lock(&locks[i]);

            // std::cout << "Thread: " << omp_get_thread_num() << "First chiEstimatePLusH value: "<< chiEstimatePlusH.GetDataPtr()[0] << std::endl;
        }
    }

    for (int j=0; j<numGridPoints ; j++ )
    {
        std::cout << "i: " << j << ", dFdx: " << dFdx[j] << std::endl;
    }
    std::cout << "Finished!!" << std::endl;

    return dFdx;
}


/*std::vector<double> OpenMPGradientDescentInversion::differential(const std::vector<std::complex<double>> &pData, PressureFieldSerial chiEstimate, const double h, const double eta)
{
    const int numGridPoints = chiEstimate.GetNumberOfGridPoints();
    std::vector<double> dFdx(numGridPoints, 0.0);
    std::vector<double> expectedDFdx= {-1.44655, -2.65177, -4.61663, -5.35039, -2.94658, -1.57442,1.35354,2.75141,3.31655,3.44341,2.76171,1.45595,0.495105,0.514024, 0.249245, 0.204169,0.543168,0.472388};

    std::cout << "\n Start parallelization of Differential with " << omp_get_max_threads() << " threads." << std::endl;
    unsigned int i;
    std::vector<double> my_dFdx;
    unsigned int blocksize;
    unsigned int offset;
    PressureFieldSerial my_chiEstimate = chiEstimate;


#pragma omp parallel shared(dFdx) private(my_chiEstimate) private(i) private (my_dFdx) private(blocksize) private(offset)
    {
        my_chiEstimate = chiEstimate;
        blocksize = dFdx.size() / omp_get_num_threads();
        int remainder = dFdx.size() % omp_get_num_threads();
        int before_remainder = std::min(omp_get_thread_num(), remainder);
        int after_remainder = std::max(0, omp_get_thread_num() - remainder);

        offset = before_remainder * (blocksize +1) + after_remainder * blocksize;
        if (before_remainder < remainder)
        {
            blocksize += 1;
        }
        if (omp_get_thread_num() == omp_get_num_threads() - 1 && offset + blocksize != dFdx.size())
        {
            std::cout << "Not all gridpoints are covered with the parallelization of the computation of differential" << std::endl;
        }

        std::vector<double> my_dFdx(blocksize);
        my_dFdx= differential_parallel(pData, my_chiEstimate, h, eta, blocksize, offset);

        std::copy(my_dFdx.begin(), my_dFdx.end(), dFdx.begin() + offset);
    }

    for (int i=0; i<numGridPoints ; i++ )
    {
        std::cout << "i: " << i << ", dFdx: " << dFdx[i] << std::endl;
    }

    return dFdx;
}*/


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
