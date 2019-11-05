#include <iostream>

#include "gradientDescentInversionInputCardReader.h"
#include "json.h"

GradientDescentInversionInputCardReader::gradientDescentInversionInputCardReader(const std::string &caseFolder)
    : inputCardReader()
{
    readCard(caseFolder);
}

GradientDescentInversionInput gradientDescentInversionInputCardReader::getInput()
{
    return _input;
}

void GradientDescentInversionInputCardReader::readCard(const std::string &caseFolder)
{
    nlohmann::json j = readFile(caseFolder + "/input/GradientDescentInversionInput.json");

    gradientDescentInversionInput input{
        j["gamma0"],
        j["x0"],
        j["h"],
        j["iter"]};

    _input = input;
}
