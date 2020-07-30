#include "randomInversion.h"
#include "log.h"
#include "progressBar.h"

namespace fwi
{
    namespace inversionMethods
    {
        RandomInversion::RandomInversion(forwardModels::ForwardModelInterface *forwardModel, const RandomInversionInput &riInput)
            : _forwardModel()
            , _riInput(riInput)
            , _grid(forwardModel->getGrid())
            , _source(forwardModel->getSource())
            , _receiver(forwardModel->getReceiver())
            , _freq(forwardModel->getFreq())
        {
            _forwardModel = forwardModel;
        }

        core::dataGrid2D RandomInversion::reconstruct(const std::vector<std::complex<double>> &pData, io::genericInput gInput)
        {
            io::progressBar bar(_riInput.nMaxInner * _riInput.nMaxOuter);

            double eta = 1.0 / (core::l2NormSquared(pData));
            double chiEstRes, newChiEstRes;

            std::ofstream residualLogFile = openResidualLogFile(gInput);

            core::dataGrid2D chiEst(_grid);

            std::vector<std::complex<double>> pDataEst(pData.size());

            _forwardModel->calculateKappa();

            // main loop//
            int counter = 1;
            for(int it = 0; it < _riInput.nMaxOuter; it++)
            {
                _forwardModel->calculatePData(chiEst, pDataEst);
                chiEstRes = _costCalculator.calculateCost(pData, pDataEst, eta);

                // start the inner loop
                for(int it1 = 0; it1 < _riInput.nMaxInner; it1++)
                {
                    core::dataGrid2D tempRandomChi(_grid);
                    tempRandomChi.randomSaurabh();

                    _forwardModel->calculatePData(chiEst, pDataEst);
                    newChiEstRes = _costCalculator.calculateCost(pData, pDataEst, eta);

                    if(it1 == 0 && it == 0)
                    {
                        tempRandomChi.copyTo(chiEst);
                        _forwardModel->calculatePData(chiEst, pDataEst);
                        chiEstRes = _costCalculator.calculateCost(pData, pDataEst, eta);
                    }
                    else if(std::abs(newChiEstRes) < std::abs(chiEstRes))
                    {
                        L_(io::linfo) << "Randomizing the temple again";
                        tempRandomChi.copyTo(chiEst);

                        _forwardModel->calculatePData(chiEst, pDataEst);
                        chiEstRes = _costCalculator.calculateCost(pData, pDataEst, eta);
                    }
                    L_(io::linfo) << it1 + 1 << "/" << _riInput.nMaxInner << "\t (" << it + 1 << "/" << _riInput.nMaxOuter
                                  << ")\t res: " << std::setprecision(17) << chiEstRes;

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

        std::ofstream RandomInversion::openResidualLogFile(io::genericInput &gInput)
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
    }   // namespace inversionMethods
}   // namespace fwi
