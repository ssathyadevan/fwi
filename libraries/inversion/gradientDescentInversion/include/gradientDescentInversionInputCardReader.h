#pragma once

#include "inputCardReader.h"
#include "gradientDescentInversionInput.h"
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
    void checkinput();
    GradientDescentInversionInput _input;
};

