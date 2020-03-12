#include "randomInversion.h"
#include "progressBar.h"
#include "log.h"

RandomInversion::RandomInversion(ForwardModelInterface *forwardModel, const RandomInversionInput &riInput)
    : _forwardModel(), _riInput(riInput), _grid(forwardModel->getGrid()), _src(forwardModel->getSrc()), _recv(forwardModel->getRecv()), _freq(forwardModel->getFreq())
{
    _forwardModel = forwardModel;
}

PressureFieldSerial RandomInversion::Reconstruct(const std::vector<std::complex<double>> &pData, GenericInput gInput)
{
    const int nTotal = _freq.nFreq * _src.nSrc * _recv.nRecv;

    ProgressBar bar(_riInput.nMaxInner * _riInput.nMaxOuter);

    double eta = 1.0 / (normSq(pData, nTotal));
    double resSq, chiEstRes, newResSq, newChiEstRes;

    PressureFieldSerial chiEst(_grid);

    chiEst.Zero();

    // open the file to store the residual log
    std::ofstream file;
    file.open(gInput.outputLocation + gInput.runName + "Residual.log", std::ios::out | std::ios::trunc);

    if (!file)
    {
        L_(lerror) << "Failed to open the file to store residuals" ;
        std::exit(EXIT_FAILURE);
    }

    int counter = 1;
    _forwardModel->calculateKappa();
    //main loop//
    for (int it = 0; it < _riInput.nMaxOuter; it++)
    {
        
        std::vector<std::complex<double>> &resArray = _forwardModel->calculateResidual(chiEst, pData);

        resSq = _forwardModel->calculateResidualNormSq(resArray);
        chiEstRes = eta * resSq;

        //start the inner loop
        for (int it1 = 0; it1 < _riInput.nMaxInner; it1++)
        {

            PressureFieldSerial tempRandomChi(_grid);
            tempRandomChi.RandomSaurabh();

            newResSq = _forwardModel->calculateResidualNormSq(_forwardModel->calculateResidual(tempRandomChi, pData));
            newChiEstRes = eta * newResSq;

            if (it1 == 0 && it == 0)
            {
                tempRandomChi.CopyTo(chiEst);
                resSq = _forwardModel->calculateResidualNormSq(_forwardModel->calculateResidual(chiEst, pData));
                chiEstRes = eta * resSq;
            }
            else if (std::abs(newChiEstRes) < std::abs(chiEstRes))
            {
                L_(linfo) << "Randomizing the temple again" ;
                tempRandomChi.CopyTo(chiEst);

                resSq = _forwardModel->calculateResidualNormSq(_forwardModel->calculateResidual(chiEst, pData));
                chiEstRes = eta * resSq;
            }
            L_(linfo) << it1 + 1 << "/" << _riInput.nMaxInner << "\t (" << it + 1 << "/" << _riInput.nMaxOuter << ")\t res: " << std::setprecision(17) << chiEstRes ;

            file << std::setprecision(17) << chiEstRes << "," << counter << std::endl;
            counter++; // store the residual value in the residual log
            bar++;
        }

        _forwardModel->calculatePTot(chiEst);
    }

    file.close(); // close the residual.log file

    PressureFieldSerial result(_grid);
    chiEst.CopyTo(result);
    return result;
}
