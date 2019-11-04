#include <iostream>

#include "gradientDescentInversionInputCardReader.h"
#include "json.h"

gradientDescentInversionInputCardReader::gradientDescentInversionInputCardReader(const std::string &caseFolder)
    : inputCardReader()
{
    readCard(caseFolder);
}

gradientDescentInversionInput gradientDescentInversionInputCardReader::getInput()
{
    return _input;
}

void gradientDescentInversionInputCardReader::readCard(const std::string &caseFolder)
{
    nlohmann::json j = readFile(caseFolder + "/input/GradientDescentInversionInput.json");

    // temporary lines
    #include <iostream>
    std::cout << "5" << std::endl;
    //

    gradientDescentInversionInput input{
        j["gamma0"],
        j["x0"],
        j["iter"]};

    _input = input;
}
