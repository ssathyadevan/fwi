#include <iostream>

#include "gradientDescentInversionInputCardReader.h"
#include "json.h"

GradientDescentInversionInputCardReader::GradientDescentInversionInputCardReader(const std::string &caseFolder)
    : InputCardReader()
{
    readCard(caseFolder);
}

GradientDescentInversionInput GradientDescentInversionInputCardReader::getInput()
{
    return _input;
}

void GradientDescentInversionInputCardReader::readCard(const std::string &caseFolder)
{
    nlohmann::json j = readFile(caseFolder + "/input/GradientDescentInversionInput.json");

    GradientDescentInversionInput input{
        j["gamma0"],
        j["x0"],
        j["h"],
        j["iter"]};

    _input = input;
}