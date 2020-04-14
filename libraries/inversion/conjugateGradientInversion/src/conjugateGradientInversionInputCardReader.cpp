#include "conjugateGradientInversionInputCardReader.h"
#include "json.h"
#include <iostream>

ConjugateGradientInversionInputCardReader::ConjugateGradientInversionInputCardReader(const std::string &caseFolder) : inputCardReader()
{
    static const std::string stringInputFolder = "/input/";
    std::string filePath = caseFolder + stringInputFolder + _fileName;

    ConjugateGradientInversionInput input;
    readJsonFile(filePath, _fileName, input);
    _input = input;
}

void ConjugateGradientInversionInputCardReader::readJsonFile(const std::string &filePath, const std::string &fileName, ConjugateGradientInversionInput &input)
{
    nlohmann::json jsonFile = readFile(filePath);

    readIterParameter(jsonFile, fileName, input);
    readDeltaAmplificationParameter(jsonFile, fileName, input);

    static const std::string parameterRegularisationEnabled = "do_reg";
    input.doRegularisation = ReadJsonHelper::tryGetBoolParameterFromJson(jsonFile, fileName, parameterRegularisationEnabled);

    static const std::string parameterMaxNrOfIterations = "n_max";
    int maxNrOfIterations = ReadJsonHelper::tryGetIntParameterFromJson(jsonFile, fileName, parameterMaxNrOfIterations);
    if(maxNrOfIterations <= 0)
    {
        throw std::invalid_argument(
            "Invalid number of iterations" + parameterMaxNrOfIterations + " (" + std::to_string(maxNrOfIterations) + " <= 0) in: " + fileName);
    }
    input.n_max = maxNrOfIterations;
}

void ConjugateGradientInversionInputCardReader::readIterParameter(
    const nlohmann::json &jsonFile, const std::string &fileName, ConjugateGradientInversionInput &input)
{
    static const std::string parameterIter = "Iter1";
    static const std::string parameterNumber = "n";
    static const std::string parameterTolerance = "tolerance";

    nlohmann::json iterObject = ReadJsonHelper::tryGetJsonObjectParameterFromJson(jsonFile, fileName, parameterIter);

    int nrOfIterations = ReadJsonHelper::tryGetIntParameterFromJson(iterObject, fileName, parameterNumber);
    if(nrOfIterations <= 0)
    {
        throw std::invalid_argument("Invalid numer of iterations (" + std::to_string(nrOfIterations) + " <= 0) in: " + fileName);
    }

    double tolerance = ReadJsonHelper::tryGetDoubleParameterFromJson(iterObject, fileName, parameterTolerance);
    if(tolerance <= 0)
    {
        throw std::invalid_argument("Invalid tolerance (" + std::to_string(tolerance) + " <= 0) in: " + fileName);
    }
    input.iteration1 = iter1(nrOfIterations, tolerance);
}

void ConjugateGradientInversionInputCardReader::readDeltaAmplificationParameter(
    const nlohmann::json &jsonFile, const std::string &fileName, ConjugateGradientInversionInput &input)
{
    static const std::string parameterDeltaAmplification = "DeltaAmplification";
    static const std::string parameterStart = "start";
    static const std::string parameterSlope = "slope";

    nlohmann::json daObject = ReadJsonHelper::tryGetJsonObjectParameterFromJson(jsonFile, fileName, parameterDeltaAmplification);

    double start = ReadJsonHelper::tryGetDoubleParameterFromJson(daObject, fileName, parameterStart);
    if(start <= 0)
    {
        throw std::invalid_argument("Invalid start step (" + std::to_string(start) + " <= 0) in: " + fileName);
    }

    double slope = ReadJsonHelper::tryGetDoubleParameterFromJson(daObject, fileName, parameterSlope);
    if(slope <= 0)
    {
        throw std::invalid_argument("Invalid slope (" + std::to_string(slope) + " <= 0) in: " + fileName);
    }
    input.dAmplification = DeltaAmplification(start, slope);
}
