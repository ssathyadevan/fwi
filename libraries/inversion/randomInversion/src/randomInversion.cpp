#include "randomInversion.h"
#include "log.h"
#include "progressBar.h"

RandomInversion::RandomInversion(forwardModelInterface *forwardModel, const RandomInversionInput &riInput) :
    _forwardModel(), _riInput(riInput), _grid(forwardModel->getGrid()), _src(forwardModel->getSrc()), _recv(forwardModel->getRecv()),
    _freq(forwardModel->getFreq())
{
    _forwardModel = forwardModel;
}

core::dataGrid2D RandomInversion::reconstruct(const std::vector<std::complex<double>> &pData, genericInput gInput)
{
    progressBar bar(_riInput.nMaxInner * _riInput.nMaxOuter);

    const int nTotal = _freq.nFreq * _src.nSrc * _recv.nRecv;
    double eta = 1.0 / (normSq(pData, nTotal));
    double resSq, chiEstRes, newResSq, newChiEstRes;

    std::ofstream residualLogFile = openResidualLogFile(gInput);

    core::dataGrid2D chiEst(_grid);

    _forwardModel->calculateKappa();

    // main loop//
    int counter = 1;
    for(int it = 0; it < _riInput.nMaxOuter; it++)
    {
        std::vector<std::complex<double>> &resArray = _forwardModel->calculateResidual(chiEst, pData);

        resSq = _forwardModel->calculateResidualNormSq(resArray);
        chiEstRes = eta * resSq;

        // start the inner loop
        for(int it1 = 0; it1 < _riInput.nMaxInner; it1++)
        {
            core::dataGrid2D tempRandomChi(_grid);
            tempRandomChi.randomSaurabh();

            newResSq = _forwardModel->calculateResidualNormSq(_forwardModel->calculateResidual(tempRandomChi, pData));
            newChiEstRes = eta * newResSq;

            if(it1 == 0 && it == 0)
            {
                tempRandomChi.copyTo(chiEst);
                resSq = _forwardModel->calculateResidualNormSq(_forwardModel->calculateResidual(chiEst, pData));
                chiEstRes = eta * resSq;
            }
            else if(std::abs(newChiEstRes) < std::abs(chiEstRes))
            {
                L_(linfo) << "Randomizing the temple again";
                tempRandomChi.copyTo(chiEst);

                resSq = _forwardModel->calculateResidualNormSq(_forwardModel->calculateResidual(chiEst, pData));
                chiEstRes = eta * resSq;
            }
            L_(linfo) << it1 + 1 << "/" << _riInput.nMaxInner << "\t (" << it + 1 << "/" << _riInput.nMaxOuter << ")\t res: " << std::setprecision(17)
                      << chiEstRes;

            residualLogFile << std::setprecision(17) << chiEstRes << "," << counter << std::endl;
            counter++;   // store the residual value in the residual log
            bar++;
        }

        _forwardModel->calculatePTot(chiEst);
    }

    residualLogFile.close();   // close the residual.log file

    core::dataGrid2D result(chiEst);
    chiEst.copyTo(result);
    return result;
}

std::ofstream RandomInversion::openResidualLogFile(genericInput &gInput)
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
