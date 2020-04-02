#include "frequenciesGroup.h"
#include "log.h"

namespace fwi
{
    static const double pi = std::atan(1.0) * 4.0;
}
frequenciesGroup::frequenciesGroup(Freq freqStruct, double c_0_) :
    nFreq(freqStruct.nTotal), c0(c_0_), dFreq((freqStruct.max - freqStruct.min) / (std::max(freqStruct.nTotal, 2) - 1)), freq(), k()
{
    if(std::abs(fwi::pi - 3.14159) > 0.0001)
    {
        throw std::invalid_argument("fwi::pi differs to much from the real value of pi.");
    }
    for(int i = 0; i < freqStruct.nTotal; i++)
    {
        freq.push_back(freqStruct.min + i * dFreq);
        double omega = 2.0 * fwi::pi * freq[i];
        k.push_back(omega / c0);
    }
}

void frequenciesGroup::Print(int nFreqTotal)
{
    L_(linfo) << "Total number of frequencies is " << nFreqTotal << ". Values:";
    for(int i = 0; i < nFreqTotal; i++)
    {
        L_(linfo) << "f = " << freq[i] << ", k = " << k[i];
    }
}
