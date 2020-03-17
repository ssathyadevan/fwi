#include <iostream>


#include "integralForwardModelInputCardReader.h"
#include "json.h"

IntegralForwardModelInputCardReader::IntegralForwardModelInputCardReader(const std::string &caseFolder)
    : InputCardReader()
{
    readCard(caseFolder);
    checkinput();
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

void IntegralForwardModelInputCardReader::checkinput()
{
    if (_input.iter2.n <= 0) {throw std::invalid_argument("Invalid number of iterations in IntegralFMInput.json.");}
    if (_input.iter2.tolerance <= 0) {throw std::invalid_argument("Invalid tolerance in IntegralFMInput.json.");}
}
