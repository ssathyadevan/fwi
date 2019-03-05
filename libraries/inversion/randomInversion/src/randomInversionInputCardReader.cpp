#include <iostream>
#include "randomInversionInputCardReader.h"
#include "json.h"

randomInversionInputCardReader::randomInversionInputCardReader(std::string inputCardPath)
    :inputCardReader()
{
    readCard(inputCardPath);
}

randomInversionInput randomInversionInputCardReader::getInput()
{
    return _input;
}


void randomInversionInputCardReader::readCard(std::string inputCardPath)
{
    std::string fileLocation = inputCardPath + "RandomInversionInput.json";
    std::ifstream in(fileLocation);

    if(!in.is_open())
    {
        std::cout << "Can't open file at " << fileLocation << std::endl;
        exit(EXIT_FAILURE);
    }

    nlohmann::json j;
    in >> j;

    randomInversionInput input
    {
        j["toleranceOuter"], j["nMaxOuter"], j["nMaxInner"]
    };

    _input = input;
}
