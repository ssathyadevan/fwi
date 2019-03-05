#pragma once
#include <vector>
#include<string>
#include "inputCardReader.h"
#include "genericInput.h"

class genericInputCardReader: public inputCardReader
{   
public:
    genericInputCardReader(std::string inputCardPath, std::string outputLocation, std::string cardName);
    genericInput getInput();

private:
    void readCard(std::string inputCardPath, std::string outputLocation, std::string cardName);

    genericInput _input;
};




