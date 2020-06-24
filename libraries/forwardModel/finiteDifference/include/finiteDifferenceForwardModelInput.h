#pragma once

#include "inputCardReader.h"
#include "pmlWidthFactor.h"
#include "sourceParameter.h"

namespace fwi
{
    namespace forwardModels
    {
        struct finiteDifferenceForwardModelInput
        {
            // For more info on input parameters, see the doc/README file
            PMLWidthFactor pmlWidthFactor;
            SourceParameter sourceParameter;
            CostFunction costFunction;
        };
    }   // namespace forwardModels
}   // namespace fwi
