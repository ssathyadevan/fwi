#ifndef CONJUGATEGRADIENTINVERSIONINPUTCARDREADER_H
#define CONJUGATEGRADIENTINVERSIONINPUTCARDREADER_H
#include "inputCardReader.h"
#include "conjugateGradientInversionInput.h"
#include <string>
#include <fstream>
#include <sstream>
class conjugateGradientInversionInputCardReader:public inputCardReader
{
public:
    conjugateGradientInversionInputCardReader(const std::string &caseFolder);
    conjugateGradientInversionInput getInput();
private:
    void readCard(const std::string &caseFolder);
    conjugateGradientInversionInput _input;
};

#endif // CONJUGATEGRADIENTINVERSIONINPUTCARDREADER_H
