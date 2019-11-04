#include <iostream>

#include "integralForwardModelInputCardReader.h"
#include "json.h"

integralForwardModelInputCardReader::integralForwardModelInputCardReader(const std::string &caseFolder)
    : inputCardReader()
{
    readCard(caseFolder);
}

integralForwardModelInput integralForwardModelInputCardReader::getInput()
{
    return _input;
}

void integralForwardModelInputCardReader::readCard(const std::string &caseFolder)
{
    nlohmann::json j = readFile(caseFolder + "/input/IntegralFMInput.json");

    // temporary lines
    #include <iostream>
    std::cout << "4" << std::endl;
    //

    integralForwardModelInput input{
        j["Iter2"]["n"], j["Iter2"]["tolerance"], j["Iter2"]["calcAlpha"]};

    _input = input;
}
