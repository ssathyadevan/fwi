#include "inversionRandom.h"
inversionRandom::inversionRandom(ForwardModelInterface *forwardModel, randomInversionInput riInput)
{
    _forwardModel = forwardModel;
    _riInput = riInput;
}


pressureFieldSerial inversionRandom::Reconstruct(const std::complex<double> *const pData, genericInput gInput)
{
    const int nTotal = gInput.freq.nTotal*
            gInput.nSourcesReceivers.rec*
            gInput.nSourcesReceivers.src;
    double eta = 1.0/(normSq(pData,nTotal));//scaling factor eq 2.10 in thesis
    double chiEstRes, randomRes;

    pressureFieldSerial chiEst(_forwardModel->getGrid() ),
            g(_forwardModel->getGrid() ), gOld(_forwardModel->getGrid() ),
            zeta(_forwardModel->getGrid() ); // stays here

    chiEst.Zero();


    // open the file to store the residual log
    std::ofstream file;
    file.open (gInput.outputLocation+gInput.cardName+"Residual.log", std::ios::out | std::ios::trunc);
    if (!file)
    {
        std::cout<< "Failed to open the file to store residuals" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    int counter = 1;
    //main loop//
    for(int it=0; it < _riInput.nMaxOuter; it++)
    {

            //forwardModel_->intermediateForwardModelStep1();
            _forwardModel->calculateResidual(chiEst, pData);
            chiEstRes = _forwardModel->calculateResidualNormSq(eta);

            //start the inner loop
            for (int it1=0; it1 < _riInput.nMaxInner; it1++)
            {

                    pressureFieldSerial tempRandomChi(_forwardModel->getGrid());
                    tempRandomChi.RandomSaurabh();
                    _forwardModel->calculateResidual(tempRandomChi, pData);
                    randomRes = _forwardModel->calculateResidualNormSq(eta);

                    if (it1 == 0)
                    {
                        tempRandomChi.CopyTo(chiEst);
                    }
                    else if (std::abs(randomRes) < std::abs(chiEstRes))
                    {
                        std::cout << "Randomizing the temple again" << std::endl;
                        tempRandomChi.CopyTo(chiEst);
                        _forwardModel->calculateResidual(chiEst, pData);
                        chiEstRes = _forwardModel->calculateResidualNormSq(eta);
                    }



                    std::cout << it1+1 << "/" << _riInput.nMaxInner << "\t (" << it+1 << "/" << _riInput.nMaxOuter << ")\t res: " << std::setprecision(17) << chiEstRes << std::endl;

                    file << std::setprecision(17) << chiEstRes << "," << counter << std::endl;
                    counter++;// store the residual value in the residual log


            }


        _forwardModel->createTotalField1D(chiEst); // estimate p_tot from the newly estimated chi (chi_est)
    }
    file.close(); // close the residual.log file

    pressureFieldSerial result(_forwardModel->getGrid());
    chiEst.CopyTo(result);
    return result;
}
