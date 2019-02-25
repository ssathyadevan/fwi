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
    forwardModelInputCardReader(std::string inputCardPath, std::string outputLocation, std::string cardName);
    forwardModelInput getInput();

private:
    virtual void readCard(std::string inputCardPath, std::string outputLocation, std::string cardName);
    forwardModelInput _input;
};
