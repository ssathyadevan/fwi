#pragma once

#include "genericInput.h"
#include "inputCardReader.h"
#include <ReadJsonHelper.h>
#include <string>
#include <vector>

class genericInputCardReader : public inputCardReader
{
public:
    genericInputCardReader(const std::string &runName);
    genericInput getInput() const { return _input; }

private:
    genericInput _input;

    const std::string _fileName = "GenericInput.json";
    static std::string setFolders(const std::string &caseFolderWithSlash, const std::string &fileName, genericInput &jsonInput);
    static void readJsonFile(const std::string &filePath, const std::string &fileName, genericInput &genericInput);

    static void readXZParametersFromJsonObject(const nlohmann::json &jsonFile, const std::string &fileName, std::array<double, 2> &array);
    static void readXZParametersFromJsonObject(const nlohmann::json &jsonFile, const std::string &fileName, std::array<int, 2> &array);

    static void readC0Parameter(const nlohmann::json &jsonFile, const std::string &fileName, genericInput &jsonInput);
    static void readFreqParameter(const nlohmann::json &jsonFile, const std::string &fileName, genericInput &jsonInput);
    static void readReservoirParameter(const nlohmann::json &jsonFile, const std::string &fileName, genericInput &jsonInput);
    static void readSourcesParameter(const nlohmann::json &jsonFile, const std::string &fileName, genericInput &jsonInput);
    static void readReceiversParameter(const nlohmann::json &jsonFile, const std::string &fileName, genericInput &jsonInput);
    static void readGridOriginalParameter(const nlohmann::json &jsonFile, const std::string &fileName, genericInput &jsonInput);
    static void readGridParameter(const nlohmann::json &jsonFile, const std::string &fileName, genericInput &jsonInput);
    static void readNSourcesParameter(const nlohmann::json &jsonFile, const std::string &fileName, genericInput &jsonInput);
    static void readNReceiversParameter(const nlohmann::json &jsonFile, const std::string &fileName, genericInput &jsonInput);
};
