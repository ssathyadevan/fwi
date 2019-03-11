#include "inversionRandom.h"
inversionRandom::inversionRandom(forwardModelBasicOptimization *forwardModel, randomInversionInput riInput)
    :m_forwardModel(), m_riInput(), m_grid(forwardModel->getGrid()), m_src(forwardModel->getSrc()), m_recv(forwardModel->getRecv()), m_freq(forwardModel->getFreq())
{
    m_forwardModel = forwardModel;
    m_riInput = riInput;
}


pressureFieldSerial inversionRandom::Reconstruct(const std::complex<double> *const pData, genericInput gInput)
{
    const int nTotal = m_freq.nFreq * m_src.nSrc * m_recv.nRecv;

    double eta = 1.0/(normSq(pData,nTotal));//scaling factor eq 2.10 in thesis
    double resSq, chiEstRes;

    pressureFieldSerial chiEst(m_grid); // stays here

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
    for(int it=0; it < m_riInput.nMaxOuter; it++)
    {
            std::complex<double>* resArray = m_forwardModel->calculateResidual(chiEst, pData);

            resSq       = m_forwardModel->calculateResidualNormSq(resArray);
            chiEstRes   = eta * resSq;

            //start the inner loop
            for (int it1 = 0; it1 < m_riInput.nMaxInner; it1++)
            {

                    pressureFieldSerial tempRandomChi(m_grid);
                    tempRandomChi.RandomSaurabh();

                    resSq       = m_forwardModel->calculateResidualNormSq(resArray);
                    chiEstRes   = eta * resSq;

                    if (it1 == 0)
                    {
                        tempRandomChi.CopyTo(chiEst);
                    }
                    else if (std::abs(resSq) < std::abs(chiEstRes))
                    {
                        std::cout << "Randomizing the temple again" << std::endl;
                        tempRandomChi.CopyTo(chiEst);

                        resSq       = m_forwardModel->calculateResidualNormSq(resArray);
                        chiEstRes   = eta * resSq;
                    }



                    std::cout << it1+1 << "/" << m_riInput.nMaxInner << "\t (" << it+1 << "/" << m_riInput.nMaxOuter << ")\t res: " << std::setprecision(17) << chiEstRes << std::endl;

                    file << std::setprecision(17) << chiEstRes << "," << counter << std::endl;
                    counter++;// store the residual value in the residual log


            }

        m_forwardModel->postProcessForwardModel(chiEst);
    }

    file.close(); // close the residual.log file

    pressureFieldSerial result(m_grid);
    chiEst.CopyTo(result);
    return result;
}
