#ifndef CONJUGATEGRADIENTINVERSIONINPUTCARDREADER_H
#define CONJUGATEGRADIENTINVERSIONINPUTCARDREADER_H
#include "inputCardReader.h"
#include "conjugateGradientInput.h"

class conjugateGradientInversionInputCardReader:public inputCardReader
{
public:
    conjugateGradientInversionInputCardReader(std::string inputCardPath, std::string outputLocation, std::string cardName);
    std::vector<std::string> Reader(const std::string &runName) const;
private:
    conjugateGradientInput readCard(std::string inputCardPath, std::string outputLocation, std::string cardName);
};

#endif // CONJUGATEGRADIENTINVERSIONINPUTCARDREADER_H
