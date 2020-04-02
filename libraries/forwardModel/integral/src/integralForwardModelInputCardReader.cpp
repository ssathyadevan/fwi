#include <iostream>


#include "integralForwardModelInputCardReader.h"
#include "json.h"

integralForwardModelInputCardReader::integralForwardModelInputCardReader(const std::string &caseFolder)
    : inputCardReader()
{
    readCard(caseFolder);
    checkInput();
}

integralForwardModelInput integralForwardModelInputCardReader::getInput()
{
    return _input;
}

void integralForwardModelInputCardReader::readCard(const std::string &caseFolder)
{
    nlohmann::json j = readFile(caseFolder + "/input/IntegralFMInput.json");

    integralForwardModelInput input{
        j["Iter2"]["n"], j["Iter2"]["tolerance"], j["Iter2"]["calcAlpha"]};

    _input = input;
}

void integralForwardModelInputCardReader::checkInput()
{
    if (_input.iter2.n <= 0) {throw std::invalid_argument("Invalid number of iterations in IntegralFMInput.json.");}
    if (_input.iter2.tolerance <= 0) {throw std::invalid_argument("Invalid tolerance in IntegralFMInput.json.");}
}
