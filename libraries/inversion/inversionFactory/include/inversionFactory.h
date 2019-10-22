#ifndef INVERSIONFACTORY_H
#define INVERSIONFACTORY_H


#include "genericInput.h"
#include "inversionInterface.h"
#include "forwardModelInterface.h"
#include <string>


class inversionFactory
{

public:
    inversionFactory() {}

    static inversionInterface* createInversion(std::string desired_inversion, ForwardModelInterface *forwardModel, const genericInput gInput);
};

#endif
