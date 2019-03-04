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
    std::ifstream in(inputCardPath + "FMInput.json");
    nlohmann::json j;
    in >> j;

    forwardModelInput jsonInput
    {
        j["Iter2"]["n"], j["Iter2"]["tolerance"], j["Iter2"]["calcAlpha"]
    };

    _input = jsonInput;
}
