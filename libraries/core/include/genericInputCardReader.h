#pragma once
#include <vector>
#include<string>
#include "inputCardReader.h"
#include "genericInput.h"


class genericInputCardReader: public inputCardReader
{   
public:
    genericInputCardReader(const std::string &pathToCardSet, const std::string &outputLocation);
    genericInput getInput();

private:
    void readCard(std::string inputCardPath);

    genericInput _input;
};
