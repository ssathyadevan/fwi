#include <iostream>

#include "gradientDescentInversionInputCardReader.h"
#include "json.h"

gradientDescentInversionInputCardReader::gradientDescentInversionInputCardReader(const std::string &caseFolder) : inputCardReader()
{
    readCard(caseFolder);
    checkInput();
}

gradientDescentInversionInput gradientDescentInversionInputCardReader::getInput() { return _input; }

void gradientDescentInversionInputCardReader::readCard(const std::string &caseFolder)
{
    nlohmann::json j = readFile(caseFolder + "/input/GradientDescentInversionInput.json");

    gradientDescentInversionInput input{j["gamma0"], j["x0"], j["h"], j["iter"]};

    _input = input;
}

void gradientDescentInversionInputCardReader::checkInput()
{
    if(_input.h <= 0)
    {
        throw std::invalid_argument("Invalid step size h in gradientDescentInversionInput.json");
    }
    if(_input.iter <= 0)
    {
        throw std::invalid_argument("Invalid numer of iterations in gradientDescentInversionInput.json");
    }
}
