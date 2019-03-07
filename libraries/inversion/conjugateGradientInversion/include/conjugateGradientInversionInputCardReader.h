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
    conjugateGradientInversionInputCardReader(const std::string &caseFolder);
    conjugateGradientInput getInput();
private:
    void readCard(const std::string &caseFolder);
    conjugateGradientInput _input;
};

#endif // CONJUGATEGRADIENTINVERSIONINPUTCARDREADER_H
