#include "evolutionInversion.h"
#include "progressBar.h"
#include "log.h"

EvolutionInversion::EvolutionInversion(ForwardModelInterface *forwardModel, const EvolutionInversionInput &eiInput)
    : _forwardModel(), _eiInput(eiInput), _grid(forwardModel->getGrid()), _src(forwardModel->getSrc()), _recv(forwardModel->getRecv()), _freq(forwardModel->getFreq())
{
    _forwardModel = forwardModel;
}

PressureFieldSerial EvolutionInversion::Reconstruct(const std::vector<std::complex<double>> &pData, GenericInput gInput)
{
    // open the file to store the residual log
    ProgressBar bar(_eiInput.nGenerations * _eiInput.nChildrenPerGeneration);
    std::ofstream file;
    file.open(gInput.outputLocation + gInput.runName + "Residual.log", std::ios::out | std::ios::trunc);

    if (!file)
    {
        L_(lerror) << "Failed to open the file to store residuals" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    int counter = 1;
    _forwardModel->calculateKappa();

    double favouriteChildResSq, childResSq, mutationRate, preParentResSq;
    mutationRate = 0.05;
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0.0,mutationRate);

    //Create initial guess, generation 0, Adam
    PressureFieldSerial parent(_grid);
    parent.RandomSaurabh();
    double parentResSq = _forwardModel->calculateResidualNormSq(_forwardModel->calculateResidual(parent, pData));
    preParentResSq = parentResSq;
    L_(linfo) << "Parent Res | Gen | Mutation Rate" << std::endl;
    std::cerr << "\n";

    //main loop// Looping through the generations
    for (int it = 0; it < _eiInput.nGenerations; it++)
    {
        PressureFieldSerial favouriteChild(_grid); //This is the best child so far
        parent.CopyTo(favouriteChild);  //The first favourite child is a clone of the parent
        favouriteChildResSq = parentResSq;
        //start the inner loop// Generating children (currently not parallel, only 1 child at a time is stored)
        for (int it1 = 0; it1 < _eiInput.nChildrenPerGeneration; it1++)
        {
            PressureFieldSerial child(_grid);
            child.RandomChild(parent, generator, distribution);
            childResSq = _forwardModel->calculateResidualNormSq(_forwardModel->calculateResidual(child, pData));

            if (childResSq < favouriteChildResSq)
            {
                child.CopyTo(favouriteChild);
                favouriteChildResSq = childResSq;
            }
            
            
            file << std::setprecision(17) << favouriteChildResSq << "," << counter << std::endl;
            counter++; // store the residual value in the residual log
            ++bar;            
        }
        L_(linfo) << std::setprecision(7) << parentResSq << "   |  "<< it << " | "<<  mutationRate << std::endl;
        //std::cerr << "\033[1A" << it << "/"<<  _eiInput.nGenerations << "(" << 100 * double(it)/double(_eiInput.nGenerations) << "%)        " ;
        
        if (favouriteChildResSq == parentResSq && preParentResSq == favouriteChildResSq){
            mutationRate /=1.1;
            distribution = std::normal_distribution<double>(0.0,mutationRate);
        }
        favouriteChild.CopyTo(parent);
        preParentResSq = parentResSq;
        parentResSq = favouriteChildResSq;
        
    }

    file.close(); // close the residual.log file

    PressureFieldSerial result(_grid);
    parent.CopyTo(result);
    return result;
}
