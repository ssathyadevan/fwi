#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "json.h"

class inputCardReader
{
public:
    virtual ~inputCardReader() = default;
protected:
    virtual void readCard(const std::string &caseFolder) = 0;

    // temporary lines
    #include <iostream>
    std::cout << "1" << std:endl;
    //

    nlohmann::json readFile(const std::string &fileName);
};

