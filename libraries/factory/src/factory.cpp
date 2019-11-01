#include "factory.h"
#include "conjugateGradientInversion.h"
#include "randomInversion.h"
#include "gradientDescentInversion.h"
#include "integralForwardModel.h"
#include "finiteDifferenceForwardModel.h"

#include <iostream>

inversionInterface *Factory::createInversion(std::string desired_inversion, ForwardModelInterface *forwardModel, const genericInput gInput)
{
        inversionInterface *inversion;
        if (desired_inversion == "conjugateGradientInversion")
        {
                inversion = new conjugateGradientInversion(forwardModel, gInput);
                return inversion;
        }

        if (desired_inversion == "randomInversion")
        {
                inversion = new randomInversion(forwardModel, gInput);
                return inversion;
        }

        if (desired_inversion == "gradientDescentInversion")
        {
                inversion = new gradientDescentInversion(forwardModel, gInput);
                return inversion;
        }
        std::cout << "The method was not found. you PUNK!" << std::endl;
        exit(EXIT_FAILURE);
}

ForwardModelInterface* Factory::createForwardModel(const genericInput &gInput, const std::string desired_forward_model, const grid2D &grid, const sources &src, const receivers &recv,
                    const frequenciesGroup &freq){
       
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