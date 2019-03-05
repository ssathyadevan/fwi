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
    randomInversionInputCardReader(std::string inputCardPath);
    randomInversionInput getInput();
private:
    randomInversionInput _input;
    void readCard(std::string inputCardPath);
};

#endif // RANDOMINVERSIONINPUTCARDREADER_H
