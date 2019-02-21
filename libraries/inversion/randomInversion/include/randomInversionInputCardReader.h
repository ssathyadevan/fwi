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
    randomInversionInputCardReader(std::string inputCardPath, std::string outputLocation, std::string cardName);
    std::vector<std::string> Reader(const std::string &runName) const;
    randomInversionInput getInput();
private:
    randomInversionInput _input;
    randomInversionInput readCard(std::string inputCardPath, std::string outputLocation, std::string cardName);
};

#endif // RANDOMINVERSIONINPUTCARDREADER_H
