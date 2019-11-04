#include "evolutionInversion.h"
EvolutionInversion::EvolutionInversion(ForwardModelInterface *forwardModel, GenericInput gInput)
    : _forwardModel(), _riInput(), _grid(forwardModel->getGrid()), _src(forwardModel->getSrc()), _recv(forwardModel->getRecv()), _freq(forwardModel->getFreq())
{
    EvolutionInversionInputCardReader EvolutionInversionInputCardReader(gInput.caseFolder);
    _forwardModel = forwardModel;
    _riInput = EvolutionInversionInputCardReader.getInput();
}

PressureFieldSerial EvolutionInversion::Reconstruct(const std::complex<double> *const pData, GenericInput gInput)
{
    const int nTotal = _freq.nFreq * _src.nSrc * _recv.nRecv;

    double eta = 1.0 / (normSq(pData, nTotal));
    double resSq, chiEstRes, newResSq, newChiEstRes;

    PressureFieldSerial chiEst(_grid);

    chiEst.Zero();

    // open the file to store the residual log
    std::ofstream file;
    file.open(gInput.outputLocation + gInput.runName + "Residual.log", std::ios::out | std::ios::trunc);

    if (!file)
    {
        std::cout << "Failed to open the file to store residuals" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    int counter = 1;
    _forwardModel->calculateKappa();
    //main loop//
    for (int it = 0; it < _riInput.nMaxOuter; it++)
    {
        
        std::complex<double> *resArray = _forwardModel->calculateResidual(chiEst, pData);

        resSq = _forwardModel->calculateResidualNormSq(resArray);
        chiEstRes = eta * resSq;

        //start the inner loop
        for (int it1 = 0; it1 < _riInput.nMaxInner; it1++)
        {

            PressureFieldSerial tempEvolutionChi(_grid);
            tempEvolutionChi.EvolutionSaurabh();

            newResSq = _forwardModel->calculateResidualNormSq(_forwardModel->calculateResidual(tempEvolutionChi, pData));
            newChiEstRes = eta * newResSq;

            if (it1 == 0 && it == 0)
            {
                tempEvolutionChi.CopyTo(chiEst);
                resSq = _forwardModel->calculateResidualNormSq(_forwardModel->calculateResidual(chiEst, pData));
                chiEstRes = eta * resSq;
            }
            else if (std::abs(newChiEstRes) < std::abs(chiEstRes))
            {
                std::cout << "Evolutionizing the temple again" << std::endl;
                tempEvolutionChi.CopyTo(chiEst);

                resSq = _forwardModel->calculateResidualNormSq(_forwardModel->calculateResidual(chiEst, pData));
                chiEstRes = eta * resSq;
            }
            std::cerr << newResSq << " | " << resSq << std::endl;
            std::cout << it1 + 1 << "/" << _riInput.nMaxInner << "\t (" << it + 1 << "/" << _riInput.nMaxOuter << ")\t res: " << std::setprecision(17) << chiEstRes << std::endl;

            file << std::setprecision(17) << chiEstRes << "," << counter << std::endl;
            counter++; // store the residual value in the residual log
        }

        _forwardModel->calculatePTot(chiEst);
    }

    file.close(); // close the residual.log file

    PressureFieldSerial result(_grid);
    chiEst.CopyTo(result);
    return result;
}
