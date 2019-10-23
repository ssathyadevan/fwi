#include <iostream>

#include "finiteDifferenceInputCardReader.h"
#include "json.h"


finiteDifferenceInputCardReader::finiteDifferenceInputCardReader(const std::string &caseFolder)
    : inputCardReader()
{
   readCard(caseFolder);
}

finiteDifferenceForwardModelInput finiteDifferenceInputCardReader::getInput()
{
    return _input;
}

void finiteDifferenceInputCardReader::readCard(const std::string &caseFolder)
{
    nlohmann::json j = readFile(caseFolder + "/input/FiniteDifferenceFMInput.json");

    finiteDifferenceForwardModelInput input
    {
        {j["PMLWidthFactor"]["x"], j["PMLWidthFactor"]["x"]},
        {j["SourceParameter"]["r"], j["SourceParameter"]["beta"]}
    };

    _input = input;
}
