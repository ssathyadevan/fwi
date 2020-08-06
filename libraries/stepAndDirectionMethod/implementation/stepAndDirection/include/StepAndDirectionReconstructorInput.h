#pragma once
#include "DirectionParameters.h"
#include "ReconstructorParameters.h"
#include "StepSizeParameters.h"

namespace fwi
{
    namespace inversionMethods
    {
        struct StepAndDirectionReconstructorInput
        {
        public:
            ReconstructorParameters reconstructorParameters;
            StepSizeParameters stepSizeParameters;
            DirectionParameters directionParameters;
        };
    }   // namespace inversionMethods
}   // namespace fwi
