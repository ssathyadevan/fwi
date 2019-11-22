#pragma once

#include "inputCardReader.h"
#include "finiteDifferenceForwardModelInput.h"

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

class FiniteDifferenceForwardModelInputCardReader: public InputCardReader
{
public:
    FiniteDifferenceForwardModelInputCardReader(const std::string &caseFolder);
    FiniteDifferenceForwardModelInput getInput();

private:
    virtual void readCard(const std::string &caseFolder);
    FiniteDifferenceForwardModelInput _input;
};
