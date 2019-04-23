#pragma once

#include "inputCardReader.h"
#include "finiteDifferenceInput.h"

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

class finiteDifferenceInputCardReader: public inputCardReader
{
public:
    finiteDifferenceInputCardReader(const std::string &caseFolder);
    finiteDifferenceInput getInput();

private:
    virtual void readCard(const std::string &caseFolder);
    finiteDifferenceInput _input;
};
