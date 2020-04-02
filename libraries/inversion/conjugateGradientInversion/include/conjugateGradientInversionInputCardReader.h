#pragma once

#include "conjugateGradientInversionInput.h"
#include "inputCardReader.h"
#include <fstream>
#include <sstream>
#include <string>

class ConjugateGradientInversionInputCardReader : public inputCardReader
{
    public:
    ConjugateGradientInversionInputCardReader(const std::string &caseFolder);
    ConjugateGradientInversionInput getInput();

    private:
    void readCard(const std::string &caseFolder) override;
    void checkInput() override;
    ConjugateGradientInversionInput _input;
};
