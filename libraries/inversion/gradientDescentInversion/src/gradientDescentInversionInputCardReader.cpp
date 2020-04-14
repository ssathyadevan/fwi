#include "gradientDescentInversionInputCardReader.h"
#include "json.h"
#include <iostream>

gradientDescentInversionInputCardReader::gradientDescentInversionInputCardReader(const std::string &caseFolder) : inputCardReader()
{
    static const std::string stringInputFolder = "/input/";
    std::string filePath = caseFolder + stringInputFolder + _fileName;

    gradientDescentInversionInput input;
    readJsonFile(filePath, _fileName, input);
    _input = input;
}

void gradientDescentInversionInputCardReader::readJsonFile(const std::string &filePath, const std::string &fileName, gradientDescentInversionInput &input)
{
    nlohmann::json jsonFile = readFile(filePath);

    static const std::string parameterGamma = "gamma0";
    input.gamma0 = ReadJsonHelper::tryGetDoubleParameterFromJson(jsonFile, fileName, parameterGamma);

    static const std::string parameterInitialX = "x0";
    input.x0 = ReadJsonHelper::tryGetDoubleParameterFromJson(jsonFile, fileName, parameterInitialX);

    static const std::string parameterStepSize = "h";
    double stepSize = ReadJsonHelper::tryGetDoubleParameterFromJson(jsonFile, fileName, parameterStepSize);
    if(stepSize <= 0)
    {
        throw std::invalid_argument("Invalid step size h (" + std::to_string(stepSize) + " <= 0) in: " + fileName);
    }
    input.h = stepSize;

    static const std::string parameterIteration = "iter";
    int nrOfIterations = ReadJsonHelper::tryGetIntParameterFromJson(jsonFile, fileName, parameterIteration);
    if(nrOfIterations <= 0)
    {
        throw std::invalid_argument("Invalid numer of iterations (" + std::to_string(nrOfIterations) + " <= 0) in: " + fileName);
    }
    input.iter = nrOfIterations;
}
