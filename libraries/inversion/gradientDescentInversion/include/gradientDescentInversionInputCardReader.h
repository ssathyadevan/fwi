#pragma once

#include "ReadJsonHelper.h"
#include "gradientDescentInversionInput.h"
#include "inputCardReader.h"
#include <fstream>
#include <sstream>
#include <string>

class gradientDescentInversionInputCardReader : public io::inputCardReader
{
private:
    gradientDescentInversionInput _input;

    const std::string _fileName = "GradientDescentInversionInput.json";
    void readJsonFile(const std::string &filePath);

public:
    gradientDescentInversionInputCardReader(const std::string &caseFolder);
    const gradientDescentInversionInput getInput() const { return _input; }
};
