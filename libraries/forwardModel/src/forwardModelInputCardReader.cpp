#include <iostream>

#include "forwardModelInputCardReader.h"
#include "json.h"


forwardModelInputCardReader::forwardModelInputCardReader(std::string inputCardPath)
    : inputCardReader()
{
   readCard(inputCardPath);
}

forwardModelInput forwardModelInputCardReader::getInput()
{
    return _input;
}

void forwardModelInputCardReader::readCard(std::string inputCardPath)
{
    std::string fileLocation = inputCardPath + "FMInput.json";
    std::ifstream in(fileLocation);

    if(!in.is_open())
    {
        std::cout << "Can't open file at " << fileLocation;
        exit(EXIT_FAILURE);
    }

    nlohmann::json j;
    in >> j;

    forwardModelInput jsonInput
    {
        j["Iter2"]["n"], j["Iter2"]["tolerance"], j["Iter2"]["calcAlpha"]
    };

    _input = jsonInput;
}
