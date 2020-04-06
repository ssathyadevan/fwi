#include <iostream>
#include "randomInversionInputCardReader.h"
#include "json.h"

RandomInversionInputCardReader::RandomInversionInputCardReader(const std::string &caseFolder)
    : inputCardReader()
{
    readCard(caseFolder);
    checkInput();
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

void RandomInversionInputCardReader::checkInput()
{
    if(_input.toleranceOuter <= 0) {throw std::invalid_argument("Invalid tolerance in RandomInversionInput.json");}
    if (_input.nMaxOuter <= 0) {throw std::invalid_argument("Invalid number of iterations nMaxOuter in RandomInversionInput.json");}
    if (_input.nMaxInner <= 0 ) {throw std::invalid_argument("Invalid number of iterations nMaxInner in RandomInversionInput.json");}
}
