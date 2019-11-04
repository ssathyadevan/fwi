#include <iostream>

#include "finiteDifferenceForwardModelInputCardReader.h"
#include "json.h"

finiteDifferenceForwardModelInputCardReader::finiteDifferenceForwardModelInputCardReader(const std::string &caseFolder)
    : inputCardReader()
{
    readCard(caseFolder);
}

finiteDifferenceForwardModelInput finiteDifferenceForwardModelInputCardReader::getInput()
{
    return _input;
}

void finiteDifferenceForwardModelInputCardReader::readCard(const std::string &caseFolder)
{
    nlohmann::json j = readFile(caseFolder + "/input/FiniteDifferenceFMInput.json");

    finiteDifferenceForwardModelInput input{
        {j["PMLWidthFactor"]["x"], j["PMLWidthFactor"]["x"]},
        {j["SourceParameter"]["r"], j["SourceParameter"]["beta"]}};

    _input = input;
}
