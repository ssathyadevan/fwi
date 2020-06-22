#pragma once

#include "StepAndDirectionReconstructorInput.h"
#include "inputCardReader.h"
#include <fstream>
#include <sstream>
#include <string>
class StepAndDirectionReconstructorInputCardReader : public io::inputCardReader
{
public:
    StepAndDirectionReconstructorInputCardReader(const std::string &caseFolder);
    const StepAndDirectionReconstructorInput getInput() const { return _input; }

private:
    const std::string _fileName = "StepAndDirectionInput.json";
    void readJsonFile(const std::string &filePath);

    void readReconstructorParameters(const nlohmann::json &jsonFile);
    void readStepSizeParameters(const nlohmann::json &jsonFile);
    void readDirectionParameters(const nlohmann::json &jsonFile);

    StepAndDirectionReconstructorInput _input;
};
