#pragma once
#include <vector>
#include<string>
#include "inputCardReader.h"
#include "genericInput.h"


class genericInputCardReader: public inputCardReader
{   
public:
    genericInputCardReader(const std::string &runName);
    genericInput getInput();

private:
    void readCard(const std::string &caseFolder);

    genericInput _input;
};




