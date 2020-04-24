#pragma once

#include "inputCardReader.h"
#include "randomInversionInput.h"
#include <fstream>
#include <sstream>
#include <string>

class RandomInversionInputCardReader : public inputCardReader
{
private:
    RandomInversionInput _input;

    const std::string _fileName = "RandomInversionInput.json";
    static void readJsonFile(const std::string &filePath, const std::string &fileName, RandomInversionInput &input);

public:
    RandomInversionInputCardReader(const std::string &caseFolder);
    const RandomInversionInput getInput() const { return _input; }
};
