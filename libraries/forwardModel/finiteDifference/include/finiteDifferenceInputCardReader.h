#pragma once

#include "inputCardReader.h"
#include "finiteDifferenceForwardModelInput.h"

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

class finiteDifferenceInputCardReader: public inputCardReader
{
public:
    finiteDifferenceInputCardReader(const std::string &caseFolder);
    finiteDifferenceForwardModelInput getInput();

private:
    virtual void readCard(const std::string &caseFolder);
    finiteDifferenceForwardModelInput _input;
};
