#pragma once

#include "inputCardReader.h"
#include "conjugateGradientInversionInput.h"
#include <string>
#include <fstream>
#include <sstream>

class ConjugateGradientInversionInputCardReader : public InputCardReader
{
public:
    ConjugateGradientInversionInputCardReader(const std::string &caseFolder);
    ConjugateGradientInversionInput getInput();
private:
    void readCard(const std::string &caseFolder);
    ConjugateGradientInversionInput _input;
};

