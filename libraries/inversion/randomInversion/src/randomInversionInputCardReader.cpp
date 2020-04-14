#include "randomInversionInputCardReader.h"
#include "ReadJsonHelper.h"
#include "json.h"
#include <iostream>

RandomInversionInputCardReader::RandomInversionInputCardReader(const std::string &caseFolder) : inputCardReader()
{
    static const std::string stringInputFolder = "/input/";
    std::string filePath = caseFolder + stringInputFolder + _fileName;

    RandomInversionInput input;
    readJsonFile(filePath, _fileName, input);
    _input = input;
}

void RandomInversionInputCardReader::readJsonFile(const std::string &filePath, const std::string &fileName, RandomInversionInput &input)
{
    nlohmann::json jsonFile = readFile(filePath);

    static const std::string parameterToleranceOuter = "toleranceOuter";
    double toleranceOuter = ReadJsonHelper::tryGetDoubleParameterFromJson(jsonFile, fileName, parameterToleranceOuter);
    if(toleranceOuter <= 0)
    {
        throw std::invalid_argument("Invalid outer tolerance (" + std::to_string(toleranceOuter) + " <= 0) in: " + fileName);
    }
    input.toleranceOuter = toleranceOuter;

    static const std::string parameterMaxIterationsInner = "nMaxInner";
    int maxIterationsInner = ReadJsonHelper::tryGetIntParameterFromJson(jsonFile, fileName, parameterMaxIterationsInner);
    if(maxIterationsInner <= 0)
    {
        throw std::invalid_argument(
            "Invalid max numer of iterations inner (" + parameterMaxIterationsInner + ") (" + std::to_string(maxIterationsInner) + " <= 0) in: " + fileName);
    }
    input.nMaxInner = maxIterationsInner;

    static const std::string parameterMaxIterationsOuter = "nMaxOuter";
    int maxIterationsOuter = ReadJsonHelper::tryGetIntParameterFromJson(jsonFile, fileName, parameterMaxIterationsOuter);
    if(maxIterationsOuter <= 0)
    {
        throw std::invalid_argument(
            "Invalid max numer of iterations outer (" + parameterMaxIterationsOuter + ") (" + std::to_string(maxIterationsOuter) + " <= 0) in: " + fileName);
    }
    input.nMaxOuter = maxIterationsOuter;
}
