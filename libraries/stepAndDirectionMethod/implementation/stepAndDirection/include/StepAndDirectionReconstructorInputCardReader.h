#pragma once

#include "StepAndDirectionReconstructorInput.h"
#include "inputCardReader.h"
#include <fstream>
#include <sstream>
#include <string>
class StepAndDirectionReconstructorInputCardReader : public inputCardReader
{
public:
    StepAndDirectionReconstructorInputCardReader(const std::string &caseFolder);
    StepAndDirectionReconstructorInput getInput();

private:
    // override void readJsonFile(const std::string &caseFolder);
    void readCard(const std::string &caseFolder);

    void checkInput() {}

    void readReconstructorParameters();
    void readStepSizeParameters();
    void readDirectionParameters();

    StepAndDirectionReconstructorInput _input;
};
