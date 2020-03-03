#pragma once

#include "inputCardReader.h"
#include "OpenMPIgradientDescentInversionInput.h"
#include <string>
#include <fstream>
#include <sstream>
class GradientDescentInversionInputCardReader:public InputCardReader
{
public:
    GradientDescentInversionInputCardReader(const std::string &caseFolder);
    GradientDescentInversionInput getInput();
private:
    void readCard(const std::string &caseFolder);
    GradientDescentInversionInput _input;
};

