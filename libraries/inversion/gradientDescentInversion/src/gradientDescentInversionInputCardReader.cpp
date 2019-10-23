#include <iostream>

#include "gradientDescentInversionInputCardReader.h"
#include "json.h"

gradientDescentInversionInputCardReader::gradientDescentInversionInputCardReader(const std::string &caseFolder)
    : inputCardReader()
{
    readCard(caseFolder);
}

gradientDescentInversionInput  gradientDescentInversionInputCardReader::getInput(){
    return _input;
}

void gradientDescentInversionInputCardReader::readCard(const std::string &caseFolder)
{
    nlohmann::json j = readFile(caseFolder + "/input/GradientDescentInput.json");

    gradientDescentInversionInput input
    {
        j["gamma"],
        j["dx"],
        j["x0"],
        j["iter"]
    };

    _input = input;
}
