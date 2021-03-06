#include "randomInversion.h"
randomInversion::randomInversion(ForwardModelInterface *forwardModel, randomInversionInput riInput)
    :_forwardModel(), _riInput(), _grid(forwardModel->getGrid()), _src(forwardModel->getSrc()), _recv(forwardModel->getRecv()), _freq(forwardModel->getFreq())
{
    _forwardModel = forwardModel;
    _riInput = riInput;
}

pressureFieldSerial randomInversion::Reconstruct(const std::complex<double> *const pData, genericInput gInput)
{
    const int nTotal = _freq.nFreq * _src.nSrc * _recv.nRecv;

    double eta = 1.0/(normSq(pData,nTotal));
    double resSq, chiEstRes;

    pressureFieldSerial chiEst(_grid);

    chiEst.Zero();

    // open the file to store the residual log
    std::ofstream file;
    file.open (gInput.outputLocation+gInput.runName+"Residual.log", std::ios::out | std::ios::trunc);

    if (!file)
    {
        std::cout<< "Failed to open the file to store residuals" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    int counter = 1;

    //main loop//
    for(int it=0; it < _riInput.nMaxOuter; it++)
    {
            std::complex<double>* resArray = _forwardModel->calculateResidual(chiEst, pData);

            resSq       = _forwardModel->calculateResidualNormSq(resArray);
            chiEstRes   = eta * resSq;

            //start the inner loop
            for (int it1 = 0; it1 < _riInput.nMaxInner; it1++)
            {

                    pressureFieldSerial tempRandomChi(_grid);
                    tempRandomChi.RandomSaurabh();

                    resSq       = _forwardModel->calculateResidualNormSq(resArray);
                    chiEstRes   = eta * resSq;

                    if (it1 == 0)
                    {
                        tempRandomChi.CopyTo(chiEst);
                    }
                    else if (std::abs(resSq) < std::abs(chiEstRes))
                    {
                        std::cout << "Randomizing the temple again" << std::endl;
                        tempRandomChi.CopyTo(chiEst);

                        resSq       = _forwardModel->calculateResidualNormSq(resArray);
                        chiEstRes   = eta * resSq;
                    }

                    std::cout << it1+1 << "/" << _riInput.nMaxInner << "\t (" << it+1 << "/" << _riInput.nMaxOuter << ")\t res: " << std::setprecision(17) << chiEstRes << std::endl;

                    file << std::setprecision(17) << chiEstRes << "," << counter << std::endl;
                    counter++;// store the residual value in the residual log
                    
            }

        _forwardModel->calculatePTot(chiEst);
    }

    file.close(); // close the residual.log file

    pressureFieldSerial result(_grid);
    chiEst.CopyTo(result);
    return result;
}
