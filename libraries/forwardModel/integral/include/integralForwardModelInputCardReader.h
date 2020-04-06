#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "inputCardReader.h"
#include "integralForwardModelInput.h"

class integralForwardModelInputCardReader : public inputCardReader
{
public:
    integralForwardModelInputCardReader(const std::string &caseFolder);
    integralForwardModelInput getInput();

private:
    virtual void readCard(const std::string &caseFolder);
    void checkInput();
    integralForwardModelInput _input;
};
