#pragma once

#include "json.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class inputCardReader
{
public:
    virtual ~inputCardReader() = default;

protected:
    virtual void readCard(const std::string &caseFolder) = 0;
    virtual void checkInput() = 0;   // TODO Remove. Check input directly

    nlohmann::json readFile(const std::string &fileName);
};
