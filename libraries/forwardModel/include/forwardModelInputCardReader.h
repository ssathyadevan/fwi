#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "inputCardReader.h"
#include "forwardModelInput.h"

class forwardModelInputCardReader: public inputCardReader
{
public:
    forwardModelInputCardReader(const std::string &caseFolder);
    forwardModelInput getInput();

private:
    virtual void readCard(const std::string &caseFolder);
    forwardModelInput _input;
};
