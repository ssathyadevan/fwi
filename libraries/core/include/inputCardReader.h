#ifndef READERVAR
#define READERVAR
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>


class inputCardReader
{
public:
    virtual ~inputCardReader() = default;
protected:
    bool InputStringToBool(std::string const& string_for_bool) const;

    std::vector<std::string> readFile(const std::string& runName) const;

    virtual void readCard(std::string inputCardPath) = 0;
};




#endif
