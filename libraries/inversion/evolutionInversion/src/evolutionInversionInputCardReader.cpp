#include <iostream>
#include "evolutionInversionInputCardReader.h"
#include "json.h"

EvolutionInversionInputCardReader::EvolutionInversionInputCardReader(const std::string &caseFolder)
    : inputCardReader()
{
    readCard(caseFolder);
    checkInput();
}

EvolutionInversionInput EvolutionInversionInputCardReader::getInput()
{
    return _input;
}

void EvolutionInversionInputCardReader::readCard(const std::string &caseFolder)
{
    nlohmann::json j = readFile(caseFolder + "/input/EvolutionInversionInput.json");

    EvolutionInversionInput input{
        j["toleranceOuter"], j["nGenerations"], j["nChildrenPerGeneration"]};

    _input = input;
}

void EvolutionInversionInputCardReader::checkInput()
{
    if (_input.toleranceOuter <= 0 ) {throw std::invalid_argument("Invalid tolerance in EvolutionInversionInput.json");}
    if (_input.nGenerations <= 0 ) {throw std::invalid_argument("Invalid number of generations in EvolutionInversionInput.json");}
    if (_input.nChildrenPerGeneration <= 0 ) { throw std::invalid_argument("Invalid number of children per generation in EvolutionInversionInput.json");}
}
