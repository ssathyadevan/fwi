#include "LinearStepSizeCalculator.h"

LinearStepSizeCalculator::LinearStepSizeCalculator(double start, double slope) : _start(start), _slope(slope)
{
    _iteration = 0;
    // throw an exception is either start or slope are < or =0
}

double LinearStepSizeCalculator::calculateStepSize()
{
    //    return _start + _iteration * _slope; //this is a linear function

    return _start / (_slope * _iteration + 1.0);   // this is an 'inverse' function, since the only parameter that gets updated is in the denominator
}

void LinearStepSizeCalculator::updateVariables(const dataGrid2D &, const dataGrid2D &, int iteration) { _iteration = iteration; }
