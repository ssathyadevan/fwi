#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "inputCardReader.h"
#include "integralForwardModelInput.h"

class IntegralForwardModelInputCardReader: public InputCardReader
{
public:
    IntegralForwardModelInputCardReader(const std::string &caseFolder);
    IntegralForwardModelInput getInput();

private:
    virtual void readCard(const std::string &caseFolder);
    void checkinput();
    IntegralForwardModelInput _input;
};
