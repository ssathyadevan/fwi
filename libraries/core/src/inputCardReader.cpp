#include "inputCardReader.h"
#include "log.h"

nlohmann::json InputCardReader::readFile(const std::string &fileName)
{
    
    std::ifstream in(fileName);

    if (!in.is_open())
    {
        L_(lwarning) << "Could not open file at " << fileName ;
        std::exit(EXIT_FAILURE);
    }

    nlohmann::json j;
    in >> j;

    return j;
}
