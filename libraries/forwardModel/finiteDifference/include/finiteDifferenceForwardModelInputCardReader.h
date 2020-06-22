#pragma once

#include "finiteDifferenceForwardModelInput.h"
#include "inputCardReader.h"
#include "ReadJsonHelper.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class finiteDifferenceForwardModelInputCardReader : public io::inputCardReader
{
public:
    finiteDifferenceForwardModelInputCardReader(const std::string &caseFolder);
    const finiteDifferenceForwardModelInput getInput() const { return _input; }

private:
    finiteDifferenceForwardModelInput _input;
    const std::string _fileName = "FiniteDifferenceFMInput.json";
    void readJsonFile(const std::string &filePath);
    void readPMLWidthFactorParameters(const nlohmann::json &jsonFile);
    void readSourceParameters(const nlohmann::json &jsonFile);
};
