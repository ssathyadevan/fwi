#include "inputCardReader.h"

bool inputCardReader::InputStringToBool(std::string const& s) const
{
    bool tmp;

    if      (s == "true"  || s[0] == '1')  { tmp = true;  }
    else if (s == "false" || s[0] == '0')  { tmp = false; }
    else                                   { exit(EXIT_FAILURE); }

    return tmp;
}

std::vector<std::string> inputCardReader::readFile(const std::string& runName) const
{
    std::vector<std::string> thevec;
    std::fstream f1(runName);

    if ( !f1.is_open() )
    {
        std::cout << "file not found" << std:: endl;
        exit(EXIT_FAILURE);
    }

    std::string line,w1;

    while ( std::getline(f1,line) )
    {
        if (( std::istringstream(line) >> w1)  && ( w1[0] != '#' ))
        {
            thevec.push_back(w1);
        }
    }

    return thevec;
}
