#include "inputCardReader.h"

nlohmann::json inputCardReader::readFile(const std::string &fileName)
{
    
    // temporary lines
    #include <iostream>
    std::cout << "1" << std::endl;
    //

    std::ifstream in(fileName);

    if (!in.is_open())
    {
        std::cout << "Could not open file at " << fileName << std::endl;
        std::exit(EXIT_FAILURE);
    }

    nlohmann::json j;
    in >> j;

    return j;
}
