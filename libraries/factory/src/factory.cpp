#include <factory.h>
#include "conjugateGradientInversion.h"
#include "randomInversion.h"
#include "evolutionInversion.h"
#include "gradientDescentInversion.h"
#include "integralForwardModel.h"
#include "finiteDifferenceForwardModel.h"

#include <iostream>

InversionInterface *Factory::createInversion(std::string desired_inversion, ForwardModelInterface *forwardModel, const GenericInput gInput)
{
        InversionInterface *inversion;
        if (desired_inversion == "conjugateGradientInversion")
        {
                inversion = new ConjugateGradientInversion(forwardModel, gInput);
                return inversion;
        }

        if (desired_inversion == "randomInversion")
        {
                inversion = new RandomInversion(forwardModel, gInput);
                return inversion;
        }

        if (desired_inversion == "gradientDescentInversion")
        {
                inversion = new GradientDescentInversion(forwardModel, gInput);
                return inversion;
        }
        if (desired_inversion == "evolutionInversion")
        {
                inversion = new EvolutionInversion(forwardModel, gInput);
                return inversion;
        }
        std::cout << "The method was not found. you PUNK!" << std::endl;
        exit(EXIT_FAILURE);
}

ForwardModelInterface* Factory::createForwardModel(const GenericInput &gInput, const std::string desired_forward_model, const Grid2D &grid, const Sources &src, const Receivers &recv,
                    const FrequenciesGroup &freq){
       
    ForwardModelInterface *model;
    if (desired_forward_model == "integralForwardModel"){
        model = new IntegralForwardModel(grid, src, recv, freq, gInput);
        return model;
    }
    if (desired_forward_model == "finiteDifferenceForwardModel"){
        model = new FiniteDifferenceForwardModel(grid, src, recv, freq, gInput);
        return model;
    }
    exit(EXIT_FAILURE);
        
}