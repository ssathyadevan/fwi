#include <iostream>

#include "integralForwardModelInputCardReader.h"
#include "json.h"

IntegralForwardModelInputCardReader::IntegralForwardModelInputCardReader(const std::string &caseFolder)
    : InputCardReader()
{
    readCard(caseFolder);
}

IntegralForwardModelInput IntegralForwardModelInputCardReader::getInput()
{
    return _input;
}

void IntegralForwardModelInputCardReader::readCard(const std::string &caseFolder)
{
    nlohmann::json j = readFile(caseFolder + "/input/IntegralFMInput.json");

    IntegralForwardModelInput input{
        j["Iter2"]["n"], j["Iter2"]["tolerance"], j["Iter2"]["calcAlpha"]};

    _input = input;
}
