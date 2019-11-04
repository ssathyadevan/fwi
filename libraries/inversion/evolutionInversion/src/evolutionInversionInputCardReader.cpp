#include <iostream>
#include "evolutionInversionInputCardReader.h"
#include "json.h"

EvolutionInversionInputCardReader::EvolutionInversionInputCardReader(const std::string &caseFolder)
    : InputCardReader()
{
    readCard(caseFolder);
}

EvolutionInversionInput EvolutionInversionInputCardReader::getInput()
{
    return _input;
}

void EvolutionInversionInputCardReader::readCard(const std::string &caseFolder)
{
    nlohmann::json j = readFile(caseFolder + "/input/EvolutionInversionInput.json");

    EvolutionInversionInput input{
        j["toleranceOuter"], j["nMaxOuter"], j["nMaxInner"]};

    _input = input;
}
