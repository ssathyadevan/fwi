#include "inversionFactory.h"
#include "conjugateGradientInversion.h"
#include "inversionRandom.h"


inversionInterface* inversionFactory::createInversion(std::string desired_inversion, ForwardModelInterface *forwardModel, const genericInput gInput){
    inversionInterface* inversion;
    if (desired_inversion == "conjugateGradient"){
            inversion = new conjugateGradientInversion(forwardModel, gInput);
            return inversion;
    }
    
    if (desired_inversion == "random"){
            inversion = new inversionRandom(forwardModel, gInput);
            return inversion;
    }
    
    exit(EXIT_FAILURE);
}
