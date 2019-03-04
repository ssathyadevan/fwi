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
    std::ifstream in(inputCardPath + "RandomInversionInput.json");
    nlohmann::json j;
    in >> j;

    randomInversionInput input
    {
        j["toleranceOuter"], j["nMaxOuter"], j["nMaxInner"]
    };

    _input = input;
}
