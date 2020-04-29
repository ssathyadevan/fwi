#include <iostream>

#include "ReadJsonHelper.h"
#include "StepAndDirectionReconstructorInputCardReader.h"
#include "json.h"

StepAndDirectionReconstructorInputCardReader::StepAndDirectionReconstructorInputCardReader(const std::string &caseFolder) : inputCardReader()
{
    const std::string stringInputFolder = "/input/";
    std::string filePath = caseFolder + stringInputFolder + _fileName;

    readJsonFile(filePath);
}

void StepAndDirectionReconstructorInputCardReader::readJsonFile(const std::string &filePath)
{
    nlohmann::json jsonFile = readFile(filePath);

    readReconstructorParameters(jsonFile);
    readStepSizeParameters(jsonFile);
    readDirectionParameters(jsonFile);

    const std::string parameterDoRegularisation = "DoRegularisation";
    const bool doRegularisation = ReadJsonHelper::tryGetParameterFromJson<bool>(jsonFile, _fileName, parameterDoRegularisation);

    _input.doConjugateGradientRegularisation = doRegularisation;
}

void StepAndDirectionReconstructorInputCardReader::readReconstructorParameters(const nlohmann::json &jsonFile)
{
    const std::string parameterReconstructor = "ReconstructorParameters";
    nlohmann::json jsonReconstructorObject = ReadJsonHelper::tryGetParameterFromJson<nlohmann::json>(jsonFile, _fileName, parameterReconstructor);

    const std::string parameterTolerance = "Tolerance";
    const double tolerance = ReadJsonHelper::tryGetParameterFromJson<double>(jsonReconstructorObject, _fileName, parameterTolerance);
    if(tolerance <= 0.0)
    {
        throw std::invalid_argument("Invalid tolerance in " + _fileName);
    }

    const std::string parameterInitialChi = "InitialChi";
    const double startingChi = ReadJsonHelper::tryGetParameterFromJson<double>(jsonReconstructorObject, _fileName, parameterInitialChi);

    const std::string parameterMaxIterationNumber = "MaxIterationNumber";
    const int maxIterationsNumber = ReadJsonHelper::tryGetParameterFromJson<int>(jsonReconstructorObject, _fileName, parameterMaxIterationNumber);
    if(maxIterationsNumber <= 0)
    {
        throw std::invalid_argument("Invalid number of iterations in " + _fileName);
    }

    _input.reconstructorParameters = ReconstructorParameters(tolerance, startingChi, maxIterationsNumber);
}

void StepAndDirectionReconstructorInputCardReader::readStepSizeParameters(const nlohmann::json &jsonFile)
{
    const std::string parameterStepSize = "StepSizeParameters";
    nlohmann::json jsonStepSizeObject = ReadJsonHelper::tryGetParameterFromJson<nlohmann::json>(jsonFile, _fileName, parameterStepSize);

    const std::string parameterInitialStepSize = "InitialStepSize";
    const double initialStepSize = ReadJsonHelper::tryGetParameterFromJson<double>(jsonStepSizeObject, _fileName, parameterInitialStepSize);
    if(initialStepSize <= 0.0)
    {
        throw std::invalid_argument("Invalid initial stepsize in " + _fileName);
    }

    const std::string parameterSlope = "Slope";
    const double slope = ReadJsonHelper::tryGetParameterFromJson<double>(jsonStepSizeObject, _fileName, parameterSlope);
    if(initialStepSize + slope * _input.reconstructorParameters.maxIterationsNumber < 0.0)
    {
        throw std::invalid_argument("Error: linear stepSize will become negative during the reconstruction, in file " + _fileName);
    }
    if(slope > 0.0)
    {
        throw std::invalid_argument("Invalid positive slope, the stepsize will increase over time, in file" + _fileName);
    }

    _input.stepSizeParameters = StepSizeParameters(initialStepSize, slope);
}

void StepAndDirectionReconstructorInputCardReader::readDirectionParameters(const nlohmann::json &jsonFile)
{
    const std::string parameterDirection = "DirectionParameters";
    nlohmann::json jsonDirectionObject = ReadJsonHelper::tryGetParameterFromJson<nlohmann::json>(jsonFile, _fileName, parameterDirection);

    const std::string parameterDerivativeStepSize = "DerivativeStepSize";
    const double derivativeStepSize = ReadJsonHelper::tryGetParameterFromJson<double>(jsonDirectionObject, _fileName, parameterDerivativeStepSize);
    if(derivativeStepSize <= 0.0)
    {
        throw std::invalid_argument("Invalid derivativeStepSize in " + _fileName);
    }
    _input.directionParameters = DirectionParameters(derivativeStepSize);
}
