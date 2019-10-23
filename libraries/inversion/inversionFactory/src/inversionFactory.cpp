#include "inversionFactory.h"
#include "conjugateGradientInversion.h"
#include "randomInversion.h"
#include "gradientDescentInversion.h"

#include <iostream>

inversionInterface* inversionFactory::createInversion(std::string desired_inversion, ForwardModelInterface *forwardModel, const genericInput gInput){
    inversionInterface* inversion;
    if (desired_inversion == "conjugateGradientInversion"){
            inversion = new conjugateGradientInversion(forwardModel, gInput);
            return inversion;
    }
    
    if (desired_inversion == "randomInversion"){
            inversion = new randomInversion(forwardModel, gInput);
            return inversion;
    }
    
    if (desired_inversion == "gradientDescentInversion"){
            inversion = new gradientDescentInversion(forwardModel, gInput);
            return inversion;
    }
    std::cout << "The method was not found. you PUNK!" << std::endl;
    exit(EXIT_FAILURE);
}
