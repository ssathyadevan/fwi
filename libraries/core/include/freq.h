#pragma once

struct Freq
{
    double min;   double max;         int nTotal;      /*double spacing;*/
    /* Suppose:
    * min = 10, max = 20, nTotal = 10, spacing = 5
    * In frequenciesGroup, this would result in frequencies exceeding the max value.
    * Is this allowed?
    */

};
