#include "evolutionInversion.h"
#include "commonVectorOperations.h"
#include "log.h"
#include "progressBar.h"

using fwi::core::operator-;

namespace fwi
{
    namespace inversionMethods
    {
        EvolutionInversion::EvolutionInversion(forwardModels::forwardModelInterface *forwardModel, const EvolutionInversionInput &eiInput)
            : _forwardModel()
            , _eiInput(eiInput)
            , _grid(forwardModel->getGrid())
            , _source(forwardModel->getSource())
            , _receiver(forwardModel->getReceiver())
            , _freq(forwardModel->getFreq())
        {
            _forwardModel = forwardModel;
        }

        core::dataGrid2D EvolutionInversion::reconstruct(const std::vector<std::complex<double>> &pData, io::genericInput gInput)
        {
            io::progressBar bar(_eiInput.nGenerations * _eiInput.nChildrenPerGeneration);

            double favouriteChildResSq, childResSq, preParentResSq;
            double mutationRate = 0.05;

            std::default_random_engine generator;
            std::normal_distribution<double> distribution(0.0, mutationRate);

            // Create initial guess, generation 0, Adam
            core::dataGrid2D parent(_grid);
            parent.randomSaurabh();
            std::vector<std::complex<double>> pDataEst(pData.size());
            _forwardModel->calculatePData(parent, pDataEst);
            double parentResSq = core::l2NormSq(pData - pDataEst);
            preParentResSq = parentResSq;
            L_(io::linfo) << "Parent Res | Gen | Mutation Rate | Convergence State" << std::endl;
            std::cerr << "\n";

            std::ofstream residualLogFile = openResidualLogFile(gInput);

            _forwardModel->calculateKappa();
            bool isConverged = false;

            // main loop// Looping through the generations
            int counter = 1;
            for(int it = 0; it < _eiInput.nGenerations; it++)
            {
                core::dataGrid2D favouriteChild(_grid);   // This is the best child so far
                parent.copyTo(favouriteChild);            // The first favourite child is a clone of the parent
                favouriteChildResSq = parentResSq;
                // start the inner loop// Generating children (currently not parallel, only 1 child at a time is stored)
                for(int it1 = 0; it1 < _eiInput.nChildrenPerGeneration; it1++)
                {
                    core::dataGrid2D child(_grid);
                    child.randomChild(parent, generator, distribution);
                    _forwardModel->calculatePData(child, pDataEst);
                    childResSq = core::l2NormSq(pData - pDataEst);

                    if(childResSq < favouriteChildResSq)
                    {
                        child.copyTo(favouriteChild);
                        favouriteChildResSq = childResSq;
                    }

                    residualLogFile << std::setprecision(17) << favouriteChildResSq << "," << counter << std::endl;
                    counter++;   // store the residual value in the residual log
                    ++bar;
                }

                isConverged = (favouriteChildResSq < _eiInput.toleranceOuter);

                std::string converganceMessage = isConverged ? "Converged" : "Not Converged";

                L_(io::linfo) << std::setprecision(7) << parentResSq << "   |  " << it << " | " << mutationRate << " | " << converganceMessage << std::endl;

                if(favouriteChildResSq == parentResSq && preParentResSq == favouriteChildResSq)
                {
                    mutationRate /= 1.1;
                    distribution = std::normal_distribution<double>(0.0, mutationRate);
                }
                favouriteChild.copyTo(parent);
                preParentResSq = parentResSq;
                parentResSq = favouriteChildResSq;
            }

            residualLogFile.close();   // close the residual.log file

            core::dataGrid2D result(_grid);
            parent.copyTo(result);
            return result;
        }

        std::ofstream EvolutionInversion::openResidualLogFile(io::genericInput &gInput)
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
