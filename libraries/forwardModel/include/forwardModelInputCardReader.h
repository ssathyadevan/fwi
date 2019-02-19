#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "inputCardReader.h"
#include "forwardModelInput.h"
class forwardModelInputCardReader: public inputCardReader
{
public:
    forwardModelInputCardReader(std::string inputCardPath, std::string outputLocation, std::string cardName);
    std::vector<std::string> Reader(const std::string &runName) const;
private:
    forwardModelInput readCard(std::string inputCardPath, std::string outputLocation, std::string cardName);
};


