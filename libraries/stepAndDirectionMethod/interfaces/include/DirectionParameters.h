#pragma once

namespace fwi
{
    namespace inversionMethods
    {
        struct DirectionParameters
        {
        public:
            double derivativeStepSize;

            DirectionParameters()
                : derivativeStepSize(0.0)
            {
            }
            DirectionParameters(double derivativeStepSize_)
                : derivativeStepSize(derivativeStepSize_)
            {
            }
        };
    }   // namespace inversionMethods
}   // namespace fwi
