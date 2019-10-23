#include "gradientDescentInversionInput.h"
#ifndef GRADIENTDESCENTINVERSIONINPUTCARDREADER_H
#define GRADIENTDESCENTINVERSIONINPUTCARDREADER_H
#include "inputCardReader.h"
#include "gradientDescentInversionInput.h"
#include <string>
#include <fstream>
#include <sstream>
class gradientDescentInversionInputCardReader:public inputCardReader
{
public:
    gradientDescentInversionInputCardReader(const std::string &caseFolder);
    gradientDescentInversionInput getInput();
private:
    void readCard(const std::string &caseFolder);
    gradientDescentInversionInput _input;
};

#endif // GRADIENTDESCENTINVERSIONINPUTCARDREADER_H
