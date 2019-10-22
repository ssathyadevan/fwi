#include "inversionFactory.h"
#include "conjugateGradientInversion.h"
#include "randomInversion.h"


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
    
    exit(EXIT_FAILURE);
}
