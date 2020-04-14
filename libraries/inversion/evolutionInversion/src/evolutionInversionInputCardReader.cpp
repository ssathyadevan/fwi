#include "evolutionInversionInputCardReader.h"
#include "json.h"
#include <iostream>

EvolutionInversionInputCardReader::EvolutionInversionInputCardReader(const std::string &caseFolder) : inputCardReader()
{
    static const std::string stringInputFolder = "/input/";
    std::string filePath = caseFolder + stringInputFolder + _fileName;

    EvolutionInversionInput input;
    readJsonFile(filePath, _fileName, input);
    _input = input;
}

void EvolutionInversionInputCardReader::readJsonFile(const std::string &filePath, const std::string &fileName, EvolutionInversionInput &input)
{
    nlohmann::json jsonFile = readFile(filePath);

    static const std::string parameterGamma = "toleranceOuter";
    double toleranceOuter = ReadJsonHelper::tryGetDoubleParameterFromJson(jsonFile, fileName, parameterGamma);
    if(toleranceOuter <= 0)
    {
        throw std::invalid_argument("Invalid outer tolerance (" + std::to_string(toleranceOuter) + " <= 0) in: " + fileName);
    }
    input.toleranceOuter = toleranceOuter;

    static const std::string parameterInitialX = "nGenerations";
    int nrOfGenerations = ReadJsonHelper::tryGetIntParameterFromJson(jsonFile, fileName, parameterInitialX);
    if(nrOfGenerations <= 0)
    {
        throw std::invalid_argument("Invalid number of generations (" + std::to_string(nrOfGenerations) + " <= 0) in: " + fileName);
    }
    input.nGenerations = nrOfGenerations;

    static const std::string parameterStepSize = "nChildrenPerGeneration";
    int nrofChildrenPerGeneration = ReadJsonHelper::tryGetIntParameterFromJson(jsonFile, fileName, parameterStepSize);
    if(nrofChildrenPerGeneration <= 0)
    {
        throw std::invalid_argument("Invalid number of children per generation (" + std::to_string(nrofChildrenPerGeneration) + " <= 0) in: " + fileName);
    }
    input.nChildrenPerGeneration = nrofChildrenPerGeneration;
}
