#include <iostream>


#include "integralForwardModelInputCardReader.h"
#include "json.h"

integralForwardModelInputCardReader::integralForwardModelInputCardReader(const std::string &caseFolder)
    : inputCardReader()
{
    const std::string stringInputFolder = "/input/";
    std::string filePath = caseFolder + stringInputFolder + _fileName;
    readJsonFile(filePath);
}

void integralForwardModelInputCardReader::readJsonFile(const std::string &filePath)
{
    nlohmann::json jsonFile = readFile(filePath);
    readIterParameters(jsonFile);
}

void integralForwardModelInputCardReader::readIterParameters(const nlohmann::json &jsonFile)
{
    const std::string parameterIter = "Iter2";
    const std::string parameterNumber = "n";
    const std::string parameterTolerance = "tolerance";
    const std::string parameterCalcAlpha = "calcAlpha";

    nlohmann::json iterObject = ReadJsonHelper::tryGetParameterFromJson<nlohmann::json>(jsonFile, _fileName, parameterIter);

    int nrOfIterations = ReadJsonHelper::tryGetParameterFromJson<int>(iterObject, _fileName, parameterNumber);
    if (nrOfIterations <= 0)
    {
        throw std::invalid_argument("Invalid number of iterations in IntegralFMInput.json.");
    }

    double tolerance = ReadJsonHelper::tryGetParameterFromJson<double>(iterObject, _fileName, parameterTolerance);
    if (tolerance <= 0)
    {
        throw std::invalid_argument("Invalid tolerance in IntegralFMInput.json.");
    }

    bool calcAlpha = ReadJsonHelper::tryGetParameterFromJson<bool>(iterObject, _fileName, parameterCalcAlpha);

    _input = integralForwardModelInput(nrOfIterations, tolerance, calcAlpha);
}
