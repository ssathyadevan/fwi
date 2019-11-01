#include <iostream>
#include "randomInversionInputCardReader.h"
#include "json.h"

RandomInversionInputCardReader::RandomInversionInputCardReader(const std::string &caseFolder)
    : InputCardReader()
{
    readCard(caseFolder);
}

RandomInversionInput RandomInversionInputCardReader::getInput()
{
    return _input;
}

void RandomInversionInputCardReader::readCard(const std::string &caseFolder)
{
    nlohmann::json j = readFile(caseFolder + "/input/RandomInversionInput.json");

    RandomInversionInput input{
        j["toleranceOuter"], j["nMaxOuter"], j["nMaxInner"]};

    _input = input;
}
