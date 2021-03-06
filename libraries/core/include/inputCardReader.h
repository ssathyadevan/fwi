#ifndef READERVAR
#define READERVAR
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

    nlohmann::json readFile(const std::string &fileName);
};

#endif
