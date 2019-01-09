#include "InversionRandom.h"

volField_rect_2D_cpu InversionRandom::Reconstruct(const std::complex<double> *const pData, Input input)
{
    double const1 = normSq(pData,forwardModel_->getInput().freq.nTotal*
                           forwardModel_->getInput().nSourcesReceivers.rec*
                           forwardModel_->getInput().nSourcesReceivers.src); // used for eta
    double eta = 1.0/const1;
    double res, previousRes;

    volField_rect_2D_cpu chiEst(forwardModel_->getGrid() ),
            g(forwardModel_->getGrid() ), gOld(forwardModel_->getGrid() ),
            zeta(forwardModel_->getGrid() ); // stays here

    chiEst.Zero();


    // open the file to store the residual log
    std::ofstream file;
    file.open (input.outputLocation+input.cardName+"Residual.log", std::ios::out | std::ios::trunc);
    if (!file)
    {
        std::cout<< "Failed to open the file to store residuals" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    int counter = 1;
    //main loop//
    for(int it=0; it < input.n_max; it++)
    {
        std::vector<std::complex<double>> vecResFirstIter;

            //forwardModel_->intermediateForwardModelStep1();
            forwardModel_->calculateResidual(chiEst, pData);
            res = forwardModel_->calculateResidualNormSq(eta);
            vecResFirstIter.push_back(res);

            //start the inner loop
            for (int it1=0; it1 < input.iter1.n; it1++)
            {

                    volField_rect_2D_cpu tempRandomChi(forwardModel_->getGrid());
                    tempRandomChi.RandomSaurabh();
                    forwardModel_->calculateResidual(tempRandomChi, pData);
                    res = forwardModel_->calculateResidualNormSq(eta);

                    if (it1 == 0)
                    {
                    tempRandomChi.CopyTo(chiEst);
                    }
                    else if (std::abs(res) > std::abs(previousRes))
                    {
                    std::cout << "Randomizing the temple again" << std::endl;
                    tempRandomChi.CopyTo(chiEst);
                    }

                    previousRes = res;

                    std::cout << it1+1 << "/" << input.iter1.n << "\t (" << it+1 << "/" << input.n_max << ")\t res: " << std::setprecision(17) << res << std::endl;

                    file << std::setprecision(17) << res << "," << counter << std::endl;
                    counter++;// store the residual value in the residual log

                    vecResFirstIter.push_back(res);

            }


        forwardModel_->createTotalField1D(input.iter2, chiEst); // estimate p_tot from the newly estimated chi (chi_est)
    }
    file.close(); // close the residual.log file

    volField_rect_2D_cpu result(forwardModel_->getGrid());
    chiEst.CopyTo(result);
    return result;
}
