#pragma once

#include "genericInput.h"
#include "inputCardReader.h"
#include <string>
#include <vector>

class genericInputCardReader : public inputCardReader
{
public:
    genericInputCardReader(const std::string &runName);
    genericInput getInput() { return _input; }

private:
    genericInput _input;

    void readCard(const std::string &caseFolder) override;
    void checkInput() override{};   // TODO: Remove

    void removeLastSlash(std::string &caseFolder);
    std::string getRunName(const std::string &caseFolder);
    void readC0Parameter(const nlohmann::json &jsonFile, genericInput &jsonInput);
    void readFreqParameter(const nlohmann::json &jsonFile, genericInput &jsonInput);
    void readReservoirParameter(const nlohmann::json &jsonFile, genericInput &jsonInput);
    void readSourcesParameter(const nlohmann::json &jsonFile, genericInput &jsonInput);
    void readReceiversParameter(const nlohmann::json &jsonFile, genericInput &jsonInput);
    void readGridOriginalParameter(const nlohmann::json &jsonFile, genericInput &jsonInput);
    void readGridParameter(const nlohmann::json &jsonFile, genericInput &jsonInput);
    void readNSourcesParameter(const nlohmann::json &jsonFile, genericInput &jsonInput);
    void readNReceiversParameter(const nlohmann::json &jsonFile, genericInput &jsonInput);
};
