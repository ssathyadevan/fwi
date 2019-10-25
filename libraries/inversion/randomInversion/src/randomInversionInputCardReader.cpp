#include <iostream>
#include "randomInversionInputCardReader.h"
#include "json.h"

randomInversionInputCardReader::randomInversionInputCardReader(const std::string &caseFolder)
    : inputCardReader()
{
    readCard(caseFolder);
}

randomInversionInput randomInversionInputCardReader::getInput()
{
    return _input;
}

void randomInversionInputCardReader::readCard(const std::string &caseFolder)
{
    nlohmann::json j = readFile(caseFolder + "/input/RandomInversionInput.json");

    randomInversionInput input{
        j["toleranceOuter"], j["nMaxOuter"], j["nMaxInner"]};

    _input = input;
}
