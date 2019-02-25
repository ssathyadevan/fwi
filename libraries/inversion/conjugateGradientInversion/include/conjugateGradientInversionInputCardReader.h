#ifndef CONJUGATEGRADIENTINVERSIONINPUTCARDREADER_H
#define CONJUGATEGRADIENTINVERSIONINPUTCARDREADER_H
#include "inputCardReader.h"
#include "conjugateGradientInput.h"
#include <string>
#include <fstream>
#include <sstream>
class conjugateGradientInversionInputCardReader:public inputCardReader
{
public:
    conjugateGradientInversionInputCardReader(std::string inputCardPath, std::string outputLocation, std::string cardName);
    conjugateGradientInput getInput();
private:
    void readCard(std::string inputCardPath, std::string outputLocation, std::string cardName);
    conjugateGradientInput _input;
};

#endif // CONJUGATEGRADIENTINVERSIONINPUTCARDREADER_H
