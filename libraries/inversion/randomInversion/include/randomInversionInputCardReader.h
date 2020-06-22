#pragma once

#include "inputCardReader.h"
#include "randomInversionInput.h"
#include <fstream>
#include <sstream>
#include <string>

class RandomInversionInputCardReader : public io::inputCardReader
{
private:
    RandomInversionInput _input;

    const std::string _fileName = "RandomInversionInput.json";
    void readJsonFile(const std::string &filePath);

public:
    RandomInversionInputCardReader(const std::string &caseFolder);
    const RandomInversionInput getInput() const { return _input; }
};
