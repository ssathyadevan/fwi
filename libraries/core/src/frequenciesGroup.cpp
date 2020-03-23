#include "frequenciesGroup.h"
#include "log.h"

namespace fwi
{
    static const double pi = std::atan(1.0) * 4.0;
}
FrequenciesGroup::FrequenciesGroup(Freq freqStruct, double c_0_) :
    nFreq(freqStruct.nTotal), c_0(c_0_), d_freq((freqStruct.max - freqStruct.min) / (std::max(freqStruct.nTotal, 2) - 1)), freq(), k()
{
    for(int i = 0; i < freqStruct.nTotal; i++)
    {
        freq.push_back(freqStruct.min + i * d_freq);
        double omega = 2.0 * fwi::pi * freq[i];
        k.push_back(omega / c_0);
    }
}

void FrequenciesGroup::Print(int nFreqTotal)
{
    L_(linfo) << "Total number of frequencies is " << nFreqTotal << ". Values:";
    for(int i = 0; i < nFreqTotal; i++)
    {
        L_(linfo) << "f = " << freq[i] << ", k = " << k[i];
    }
}
