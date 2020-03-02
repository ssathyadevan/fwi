#include <iostream>

#include "finiteDifferenceForwardModelInputCardReader.h"
#include "json.h"

FiniteDifferenceForwardModelInputCardReader::FiniteDifferenceForwardModelInputCardReader(const std::string &caseFolder)
    : InputCardReader()
{
    readCard(caseFolder);
}

FiniteDifferenceForwardModelInput FiniteDifferenceForwardModelInputCardReader::getInput()
{
    return _input;
}

void FiniteDifferenceForwardModelInputCardReader::readCard(const std::string &caseFolder)
{
    nlohmann::json j = readFile(caseFolder + "/input/FiniteDifferenceFMInput.json");

    FiniteDifferenceForwardModelInput input{
        {j["PMLWidthFactor"]["x"], j["PMLWidthFactor"]["x"]},
        {j["SourceParameter"]["r"], j["SourceParameter"]["beta"]}};

    _input = input;
}

void FiniteDifferenceForwardModelInputCardReader::checkinput()
{

}
