#include "frequenciesGroup.h"

FrequenciesGroup::FrequenciesGroup(Freq freqStruct, double c_0_)
    : nFreq(freqStruct.nTotal), c_0(c_0_), d_freq((freqStruct.max-freqStruct.min) / (freqStruct.nTotal - 1)), freq(), k()
{
    assert(nFreq > 1);
    for (int i = 0; i < freqStruct.nTotal; i++)
    {
        freq.push_back(freqStruct.min + i * d_freq);
        double omega = 2.0 * M_PI * freq[i];
        k.push_back(omega / c_0);
    }
}

void FrequenciesGroup::Print(int nFreqTotal)
{
    std::cout << "Total number of frequencies is " << nFreqTotal << ". Values:" << std::endl;
    for (int i = 0; i < nFreqTotal; i++)
    {
        std::cout << "f = " << freq[i] << ", k = " << k[i] << std::endl;
    }
    std::cout << std::endl;
}
