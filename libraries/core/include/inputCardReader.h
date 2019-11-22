#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "json.h"

class InputCardReader
{
public:
    virtual ~InputCardReader() = default;
protected:
    virtual void readCard(const std::string &caseFolder) = 0;

    nlohmann::json readFile(const std::string &fileName);
};

