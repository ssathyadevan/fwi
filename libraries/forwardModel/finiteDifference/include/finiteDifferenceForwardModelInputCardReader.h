#pragma once

#include "inputCardReader.h"
#include "finiteDifferenceForwardModelInput.h"

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

class finiteDifferenceForwardModelInputCardReader : public inputCardReader
{
public:
    finiteDifferenceForwardModelInputCardReader(const std::string &caseFolder);
    finiteDifferenceForwardModelInput getInput();

private:
    virtual void readCard(const std::string &caseFolder);
    void checkInput();
    finiteDifferenceForwardModelInput _input;
};
