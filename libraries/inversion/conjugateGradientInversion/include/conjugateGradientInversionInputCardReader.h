#pragma once

#include "ReadJsonHelper.h"
#include "conjugateGradientInversionInput.h"
#include "inputCardReader.h"
#include <fstream>
#include <sstream>
#include <string>

class ConjugateGradientInversionInputCardReader : public inputCardReader
{
private:
    ConjugateGradientInversionInput _input;

    const std::string _fileName = "ConjugateGradientInversionInput.json";
    static void readJsonFile(const std::string &filePath, const std::string &fileName, ConjugateGradientInversionInput &input);

    static void readIterParameter(const nlohmann::json &jsonFile, const std::string &fileName, ConjugateGradientInversionInput &jsonInput);
    static void readDeltaAmplificationParameter(const nlohmann::json &jsonFile, const std::string &fileName, ConjugateGradientInversionInput &input);

public:
    ConjugateGradientInversionInputCardReader(const std::string &caseFolder);
    const ConjugateGradientInversionInput getInput() const { return _input; }
};
