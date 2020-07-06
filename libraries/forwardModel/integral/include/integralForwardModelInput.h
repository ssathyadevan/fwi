#pragma once

#include "costFunction.h"

namespace fwi
{
    namespace forwardModels
    {
        struct integralForwardModelInput
        {
            int nrOfIterations;
            double tolerance;
            bool calcAlpha;
            CostFunction costFunction;

            integralForwardModelInput()
                : nrOfIterations(0)
                , tolerance(0.0)
                , calcAlpha(false)
                , costFunction(leastSquares)
            {
            }
            integralForwardModelInput(int nrOfIterations_, double tolerance_, bool calcAlpha_, CostFunction costFunction_)
                : nrOfIterations(nrOfIterations_)
                , tolerance(tolerance_)
                , calcAlpha(calcAlpha_)
                , costFunction(costFunction_)
            {
            }
        };
    }   // namespace forwardModels
}   // namespace fwi
