#pragma once

#include "inputCardReader.h"
#include "randomInversionInput.h"
#include <string>
#include <fstream>
#include <sstream>

class RandomInversionInputCardReader: public InputCardReader
{
public:
    RandomInversionInputCardReader(const std::string &caseFolder);
    RandomInversionInput getInput();
private:
    RandomInversionInput _input;
    void readCard(const std::string &caseFolder);
};
