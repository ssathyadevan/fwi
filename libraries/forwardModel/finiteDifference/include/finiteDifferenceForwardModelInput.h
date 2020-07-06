#pragma once

#include "costFunction.h"
#include "pmlWidthFactor.h"
#include "sourceParameter.h"

#include <string>

namespace fwi
{
    namespace forwardModels
    {
        enum BoundaryConditionType
        {
            PML, // Perfectly Matched Layers
            FirstOrderABC, // First Order Absorbing Boundary Layers
            SecondOrderABC // Second Order Absorbing Boundary Layers
        };

        struct finiteDifferenceForwardModelInput
        {
            PMLWidthFactor pmlWidthFactor;
            SourceParameter sourceParameter;
            CostFunction costFunction;
            BoundaryConditionType boundaryConditionType;
        };
    }   // namespace forwardModels
}   // namespace fwi
