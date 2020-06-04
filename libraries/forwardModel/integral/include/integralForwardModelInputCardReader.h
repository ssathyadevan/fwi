#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "inputCardReader.h"
#include "integralForwardModelInput.h"
#include "ReadJsonHelper.h"

class integralForwardModelInputCardReader : public inputCardReader
{
public:
    integralForwardModelInputCardReader(const std::string &caseFolder);
    const integralForwardModelInput getInput() const { return _input; }

private:
    const std::string _fileName = "IntegralFMInput.json";
    integralForwardModelInput _input;
    void readJsonFile(const std::string &filePath);
    void readIterParameters(const nlohmann::json &jsonFile);
};
