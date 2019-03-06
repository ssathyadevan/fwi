#include <iostream>

#include "forwardModelInputCardReader.h"
#include "json.h"


forwardModelInputCardReader::forwardModelInputCardReader(const std::string &caseFolder)
    : inputCardReader()
{
   readCard(caseFolder);
}

forwardModelInput forwardModelInputCardReader::getInput()
{
    return _input;
}

void forwardModelInputCardReader::readCard(const std::string &caseFolder)
{
    nlohmann::json j = readFile(caseFolder + "/input/FMInput.json");

    forwardModelInput input
    {
        j["Iter2"]["n"], j["Iter2"]["tolerance"], j["Iter2"]["calcAlpha"]
    };

    _input = input;
}
