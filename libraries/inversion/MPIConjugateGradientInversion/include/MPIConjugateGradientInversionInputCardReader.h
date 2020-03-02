#pragma once

#include "inputCardReader.h"
#include "MPIConjugateGradientInversionInput.h"
#include <string>
#include <fstream>
#include <sstream>

class MPIConjugateGradientInversionInputCardReader : public InputCardReader
{
public:
    MPIConjugateGradientInversionInputCardReader(const std::string &caseFolder);
    MPIConjugateGradientInversionInput getInput();
private:
    void readCard(const std::string &caseFolder);
    void checkinput();
    MPIConjugateGradientInversionInput _input;
};

