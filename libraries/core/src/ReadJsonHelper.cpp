#include "ReadJsonHelper.h"
#include "json.h"
#include <exception>
#include <iostream>
#include <string.h>

void ReadJsonHelper::verifyJsonFileContainsParameter(const nlohmann::json &jsonFile, const std::string &filePath, const std::string &parameterName)
{
    if(!jsonFile.contains(parameterName))
    {
        throw std::invalid_argument("Argument " + parameterName + " is missing in: " + filePath);
    }
}

nlohmann::json ReadJsonHelper::tryGetJsonObjectParameterFromJson(const nlohmann::json &jsonFile, const std::string &filePath, const std::string &parameterName)
{
    verifyJsonFileContainsParameter(jsonFile, filePath, parameterName);
    return jsonFile[parameterName];
}

bool ReadJsonHelper::tryGetBoolParameterFromJson(const nlohmann::json &jsonFile, const std::string &filePath, const std::string &parameterName)
{
    verifyJsonFileContainsParameter(jsonFile, filePath, parameterName);
    return jsonFile[parameterName];
}

int ReadJsonHelper::tryGetIntParameterFromJson(const nlohmann::json &jsonFile, const std::string &filePath, const std::string &parameterName)
{
    verifyJsonFileContainsParameter(jsonFile, filePath, parameterName);
    return jsonFile[parameterName];
}

double ReadJsonHelper::tryGetDoubleParameterFromJson(const nlohmann::json &jsonFile, const std::string &filePath, const std::string &parameterName)
{
    verifyJsonFileContainsParameter(jsonFile, filePath, parameterName);
    return jsonFile[parameterName];
}

std::string ReadJsonHelper::tryGetStringParameterFromJson(const nlohmann::json &jsonFile, const std::string &filePath, const std::string &parameterName)
{
    verifyJsonFileContainsParameter(jsonFile, filePath, parameterName);
    return jsonFile[parameterName];
}
