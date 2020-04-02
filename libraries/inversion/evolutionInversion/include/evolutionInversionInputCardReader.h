#pragma once

#include "inputCardReader.h"
#include "evolutionInversionInput.h"
#include <string>
#include <fstream>
#include <sstream>

class EvolutionInversionInputCardReader: public inputCardReader
{
public:
    EvolutionInversionInputCardReader(const std::string &caseFolder);
    EvolutionInversionInput getInput();
private:
    EvolutionInversionInput _input;
    void readCard(const std::string &caseFolder);
    void checkInput();
};
