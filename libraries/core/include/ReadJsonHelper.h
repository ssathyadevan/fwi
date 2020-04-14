#pragma once

#include "json.h"
#include <string>
#include <vector>

class ReadJsonHelper
{
public:
    static void verifyJsonFileContainsParameter(const nlohmann::json &jsonFile, const std::string &filePath, const std::string &parameterName);
    static nlohmann::json tryGetJsonObjectParameterFromJson(const nlohmann::json &jsonFile, const std::string &filePath, const std::string &parameterName);
    static bool tryGetBoolParameterFromJson(const nlohmann::json &jsonFile, const std::string &filePath, const std::string &parameterName);
    static int tryGetIntParameterFromJson(const nlohmann::json &jsonFile, const std::string &filePath, const std::string &parameterName);
    static double tryGetDoubleParameterFromJson(const nlohmann::json &jsonFile, const std::string &filePath, const std::string &parameterName);
    static std::string tryGetStringParameterFromJson(const nlohmann::json &jsonFile, const std::string &filePath, const std::string &parameterName);
};
