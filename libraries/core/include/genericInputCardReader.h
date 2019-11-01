#pragma once

#include <vector>
#include <string>
#include "inputCardReader.h"
#include "genericInput.h"


class GenericInputCardReader: public InputCardReader
{   
public:
    GenericInputCardReader(const std::string &runName);
    GenericInput getInput();

private:
    void readCard(const std::string &caseFolder);

    GenericInput _input;
};
