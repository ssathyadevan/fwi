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
    virtual void readCard(std::string inputCardPath) = 0;
};




#endif
