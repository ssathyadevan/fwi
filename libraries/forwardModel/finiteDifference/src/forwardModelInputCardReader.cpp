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
    nlohmann::json j = readFile(caseFolder + "/input/FiniteDifferenceFMInput.json");

    forwardModelInput input
    {
        {j["PMLWidthFactor"]["x"], j["PMLWidthFactor"]["x"]},
        {j["SourceParameter"]["r"], j["SourceParameter"]["beta"]}
    };

    _input = input;
}
