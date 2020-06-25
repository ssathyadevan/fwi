#include "LinearStepSizeCalculator.h"

namespace fwi
{
    namespace inversionMethods
    {
        LinearStepSizeCalculator::LinearStepSizeCalculator(double start, double slope)
            : _start(start)
            , _slope(slope)
            , _iteration(0)
        {
            if(start <= 0.0 || slope <= 0.0)
            {
                throw std::invalid_argument("Invalid parameters for step choice.");
            }
        }

        LinearStepSizeCalculator::~LinearStepSizeCalculator() {}

        double LinearStepSizeCalculator::calculateStepSize()
        {
            //    return _start + _iteration * _slope; //this is a linear function

            return _start / (_slope * _iteration + 1.0);   // this is an 'inverse' function, since the only parameter that gets updated is in the denominator
        }
    }   // namespace inversionMethods
}   // namespace fwi
