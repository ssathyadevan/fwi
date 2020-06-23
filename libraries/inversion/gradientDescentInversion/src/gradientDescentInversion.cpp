#include "gradientDescentInversion.h"
#include "gradientDescentInversionInputCardReader.h"
#include "log.h"
#include "progressBar.h"

namespace inversionMethods
{
    gradientDescentInversion::gradientDescentInversion(forwardModels::forwardModelInterface *forwardModel, const gradientDescentInversionInput &gdInput)
        : _forwardModel()
        , _gdInput(gdInput)
        , _grid(forwardModel->getGrid())
        , _src(forwardModel->getSrc())
        , _recv(forwardModel->getRecv())
        , _freq(forwardModel->getFreq())
    {
        _forwardModel = forwardModel;
    }

    core::dataGrid2D gradientDescentInversion::reconstruct(const std::vector<std::complex<double>> &pData, io::genericInput gInput)
    {
        io::progressBar bar(_gdInput.iter);
        std::ofstream residualLogFile = openResidualLogFile(gInput);

        core::dataGrid2D chiEstimateCurrent(_grid);
        chiEstimateCurrent = _gdInput.x0;
        core::dataGrid2D chiEstimatePrevious(_grid);

        _forwardModel->calculateKappa();
        _forwardModel->calculateResidual(chiEstimateCurrent, pData);
        std::vector<std::complex<double>> residual = _forwardModel->calculateResidual(chiEstimateCurrent, pData);

        std::vector<double> dFdxCurrent(_grid.getNumberOfGridPoints(), 0);
        std::vector<double> dFdxPrevious;

        double fx;
        bool isConverged = false;
        int counter = 1;
        const int nTotal = _freq.nFreq * _src.nSrc * _recv.nRecv;
        double eta = 1.0 / (forwardModels::normSq(pData, nTotal));
        double gamma = _gdInput.gamma0;   // First iteration

        for(int it1 = 0; it1 < _gdInput.iter; it1++)
        {
            dFdxPrevious = dFdxCurrent;
            dFdxCurrent = differential(residual, chiEstimateCurrent, pData, eta, _gdInput.h);

            if(it1 > 0)
            {
                gamma = determineGamma(chiEstimatePrevious, chiEstimateCurrent, dFdxPrevious, dFdxCurrent);
            }

            chiEstimatePrevious = chiEstimateCurrent;
            chiEstimateCurrent = gradientDescent(chiEstimateCurrent, dFdxCurrent, gamma);
            fx = _forwardModel->calculateCost(residual, chiEstimateCurrent, pData, eta);
            isConverged = (fx < _gdInput.h);
            logResidualResults(counter, fx, isConverged);
            residualLogFile << std::setprecision(17) << fx << "," << counter << std::endl;

            ++counter;
            bar++;
        }

        residualLogFile.close();
        return chiEstimateCurrent;
    }

    std::ofstream gradientDescentInversion::openResidualLogFile(io::genericInput &gInput)
    {
        std::string filePath = gInput.outputLocation + gInput.runName + "Residual" + ".log";

        std::ofstream residualLogFile;
        residualLogFile.open(filePath, std::ios::out | std::ios::trunc);
        if(!residualLogFile)
        {
            throw std::invalid_argument("Unable to store residuals from file : " + filePath);
        }

        return residualLogFile;
    }

    std::vector<double> gradientDescentInversion::differential(
        std::vector<std::complex<double>> &residual, core::dataGrid2D chiEstimate, const std::vector<std::complex<double>> &pData, double eta, double h)
    {
        const int numGridPoints = chiEstimate.getNumberOfGridPoints();

        double fx = _forwardModel->calculateCost(residual, chiEstimate, pData, eta);

        double fxPlusH;
        core::dataGrid2D chiEstimatePlusH(chiEstimate);
        std::vector<double> dFdx(numGridPoints, 0.0);
        for(int i = 0; i < numGridPoints; ++i)
        {
            chiEstimatePlusH.addValueAtIndex(h, i);   // Add h
            fxPlusH = _forwardModel->calculateCost(residual, chiEstimatePlusH, pData, eta);
            chiEstimatePlusH.addValueAtIndex(-h, i);   // Remove h

            dFdx[i] = (fxPlusH - fx) / h;
        }

        return dFdx;
    }

    core::dataGrid2D gradientDescentInversion::gradientDescent(core::dataGrid2D chiEstimate, const std::vector<double> &dfdx, const double gamma)
    {
        const int nGridPoints = chiEstimate.getNumberOfGridPoints();

        std::vector<double> descentVector(nGridPoints, 0.0);
        for(int i = 0; i < nGridPoints; ++i)
        {
            descentVector[i] = -1 * gamma * dfdx[i];
        }
        chiEstimate += descentVector;

        return chiEstimate;
    }

    void gradientDescentInversion::logResidualResults(int iteration, double residual, bool isConverged)
    {
        std::string convergenceMessage = isConverged ? "Converged" : "Not Converged";
        L_(io::linfo) << "Iteration:" << iteration << "\t res: " << std::setprecision(17) << residual << "\t" << convergenceMessage;
    }

    double gradientDescentInversion::determineGamma(const core::dataGrid2D chiEstimatePrevious, const core::dataGrid2D chiEstimateCurrent,
        std::vector<double> dFdxPrevious, std::vector<double> dFdxCurrent)
    {
        const int nGridPoints = chiEstimateCurrent.getNumberOfGridPoints();

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
}   // namespace inversionMethods
