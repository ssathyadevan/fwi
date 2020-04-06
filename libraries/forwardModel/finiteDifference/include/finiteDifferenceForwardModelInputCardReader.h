#pragma once

#include "finiteDifferenceForwardModelInput.h"
#include "inputCardReader.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class finiteDifferenceForwardModelInputCardReader : public inputCardReader
{
public:
    finiteDifferenceForwardModelInputCardReader(const std::string &caseFolder);
    finiteDifferenceForwardModelInput getInput() { return _input; }

private:
    virtual void readCard(const std::string &caseFolder);
    void checkInput();
    finiteDifferenceForwardModelInput _input;
};
