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
            PMLWidthFactor pmlWidthFactor;
            SourceParameter sourceParameter;
            CostFunction costFunction;
        };
    }   // namespace forwardModels
}   // namespace fwi
