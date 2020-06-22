#pragma once

#include <cassert>
#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include "freqInfo.h"
#include <cmath>

namespace core
{
    class frequenciesGroup
    {
        frequenciesGroup operator=(const frequenciesGroup &) = delete;

    public:
        const int nFreq;
        const double c0;
        const double dFreq;
        std::vector<double> freq;
        std::vector<double> k;

        frequenciesGroup(freqInfo freqStruct, double c_0_);

        void Print(int nFreqTotal);
    };
}   // namespace core
