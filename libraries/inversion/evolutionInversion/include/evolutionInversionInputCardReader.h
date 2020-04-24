#pragma once

#include "evolutionInversionInput.h"
#include "inputCardReader.h"
#include <ReadJsonHelper.h>
#include <fstream>
#include <sstream>
#include <string>

class EvolutionInversionInputCardReader : public inputCardReader
{
public:
    EvolutionInversionInputCardReader(const std::string &caseFolder);
    const EvolutionInversionInput getInput() const { return _input; }

private:
    EvolutionInversionInput _input;

    const std::string _fileName = "EvolutionInversionInput.json";
    static void readJsonFile(const std::string &filePath, const std::string &fileName, EvolutionInversionInput &input);
};
