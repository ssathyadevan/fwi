#include "OpenMPgradientDescentInversion.h"
#include "gradientDescentInversionInputCardReader.h"
#include "progressBar.h"
#include <iostream>
#include <omp.h>
#include <sstream>

namespace fwi
{
    namespace inversionMethods
    {
        OpenMPGradientDescentInversion::OpenMPGradientDescentInversion(const io::genericInput &gInput, ForwardModelContainer &forwardmodels)
            : _forwardModelsParallel(forwardmodels)
            , _gdInput()
            , _grid(forwardmodels.getGrid())
            , _source(forwardmodels.getSources())
            , _receiver(forwardmodels.getReceivers())
            , _freq(forwardmodels.getFrequencies())
        {
            gradientDescentInversionInputCardReader GradientDescentInversionReader(gInput.caseFolder);
            _gdInput = GradientDescentInversionReader.getInput();
        }

        core::dataGrid2D OpenMPGradientDescentInversion::reconstruct(const std::vector<std::complex<double>> &pData, io::genericInput gInput)
        {
            io::progressBar bar(_gdInput.iter);

            std::ofstream residualLogFile = openResidualLogFile(gInput);

            core::dataGrid2D chiEstimateCurrent(_grid);
            chiEstimateCurrent = _gdInput.x0;
            core::dataGrid2D chiEstimatePrevious(_grid);

            _forwardModelsParallel.calculateKappaParallel();

            std::vector<double> dFdxCurrent(_grid.getNumberOfGridPoints(), 0);
            std::vector<double> dFdxPrevious;

            double fx;
            int counter = 1;
            const int nTotal = _freq.nFreq * _source.count * _receiver.count;
            double eta = 1.0 / (forwardModels::normSq(pData, nTotal));
            double gamma = _gdInput.gamma0;   // First iteration
            for(int it1 = 0; it1 < _gdInput.iter; it1++)
            {
                dFdxPrevious = dFdxCurrent;
                dFdxCurrent = differentialParallel(pData, chiEstimateCurrent, _gdInput.h, eta);

                if(it1 > 0)
                {
                    gamma = determineGamma(chiEstimatePrevious, chiEstimateCurrent, dFdxPrevious, dFdxCurrent);
                }

                chiEstimatePrevious = chiEstimateCurrent;
                chiEstimateCurrent = gradientDescent(chiEstimateCurrent, dFdxCurrent, gamma);
                fx = functionFParallel(chiEstimateCurrent, pData, eta);
                residualLogFile << std::setprecision(17) << fx << "," << counter << std::endl;
                _forwardModelsParallel.calculateKappaParallel();
                ++counter;
                bar++;
            }

            residualLogFile.close();
            return chiEstimateCurrent;
        }

        std::ofstream OpenMPGradientDescentInversion::openResidualLogFile(io::genericInput &gInput)
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

        double OpenMPGradientDescentInversion::functionFParallel(const core::dataGrid2D &xi, const std::vector<std::complex<double>> &pData, double eta)
        {
            return eta * _forwardModelsParallel.calculateResidualNormSqParallel(_forwardModelsParallel.calculateResidualParallel(xi, pData));
        }

        core::dataGrid2D OpenMPGradientDescentInversion::gradientDescent(core::dataGrid2D chiEstimate, const std::vector<double> &dfdx, double gamma)
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

        std::vector<double> OpenMPGradientDescentInversion::differentialParallel(
            const std::vector<std::complex<double>> &pData, core::dataGrid2D chiEstimate, double h, double eta)
        {
            int numGridPoints = chiEstimate.getNumberOfGridPoints();
            double FxPlusH;
            std::vector<double> dFdx(numGridPoints);
            double Fx = functionFParallel(chiEstimate, pData, eta);
            core::dataGrid2D chiEstimatePlusH = chiEstimate;

            for(int i = 0; i != numGridPoints; ++i)
            {
                chiEstimatePlusH.setValueAtIndex(h, i);
                FxPlusH = functionFParallel(chiEstimatePlusH, pData, eta);
                dFdx[i] = (FxPlusH - Fx) / h;
                chiEstimatePlusH.setValueAtIndex(-h, i);
            }
            return dFdx;
        }

        double OpenMPGradientDescentInversion::determineGamma(const core::dataGrid2D chiEstimatePrevious, const core::dataGrid2D chiEstimateCurrent,
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
}   // namespace fwi
