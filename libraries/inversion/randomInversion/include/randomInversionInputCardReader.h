#ifndef RANDOMINVERSIONINPUTCARDREADER_H
#define RANDOMINVERSIONINPUTCARDREADER_H
#include "inputCardReader.h"
#include "randomInversionInput.h"
#include <string>
#include <fstream>
#include <sstream>

class randomInversionInputCardReader: public inputCardReader
{
public:
    randomInversionInputCardReader(const std::string &caseFolder);
    randomInversionInput getInput();
private:
    randomInversionInput _input;
    void readCard(const std::string &caseFolder);
};

#endif // RANDOMINVERSIONINPUTCARDREADER_H
