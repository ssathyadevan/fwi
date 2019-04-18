#ifndef FOWARDMODELINPUT_H
#define FOWARDMODELINPUT_H

#include "inputStructs.h"

struct forwardModelInput
{
    // For more info on input parameters, see the doc/README file
    PMLWidthFactor pmlWidthFactor;
    SourceParameter sourceParameter;
};

#endif
