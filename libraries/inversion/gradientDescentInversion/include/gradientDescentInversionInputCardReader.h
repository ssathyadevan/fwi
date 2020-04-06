#pragma once

#include "inputCardReader.h"
#include "gradientDescentInversionInput.h"
#include <string>
#include <fstream>
#include <sstream>
class gradientDescentInversionInputCardReader :public inputCardReader
{
public:
    gradientDescentInversionInputCardReader(const std::string &caseFolder);
    gradientDescentInversionInput getInput();
private:
    void readCard(const std::string &caseFolder);
    void checkInput();
    gradientDescentInversionInput _input;
};

