#pragma once

#include "inputStructs.h"

namespace forwardModels
{
    struct finiteDifferenceForwardModelInput
    {
        // For more info on input parameters, see the doc/README file
        PMLWidthFactor pmlWidthFactor;
        SourceParameter sourceParameter;
    };
}   // namespace forwardModels
