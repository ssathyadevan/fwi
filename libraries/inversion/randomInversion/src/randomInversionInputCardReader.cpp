#include "randomInversionInputCardReader.h"

randomInversionInputCardReader::randomInversionInputCardReader(std::string inputCardPath, std::string outputLocation, std::string cardName):inputCardReader()
{
    _input = readCard(inputCardPath,outputLocation,cardName);
}

randomInversionInput randomInversionInputCardReader::getInput()
{
    return _input;
}
std::vector<std::string> randomInversionInputCardReader::Reader(const std::string& runName) const
{
    std::vector<std::string> thevec;
    std::fstream f1(runName);
    std::string line,w1;
    while ( getline(f1,line) )
    {
        if (( std::istringstream(line) >> w1)  && ( w1[0] != '#' ))
        {
            thevec.push_back(w1);
        }
    }
    return thevec;
}

randomInversionInput randomInversionInputCardReader::readCard(std::string inputCardPath, std::string outputLocation, std::string cardName)
{
    std::string filePath = inputCardPath+cardName+".in";
    std::vector<std::string> input_parameters = Reader(filePath);
    int parameterCounter=0;
    const double toleratnceOuter                      = stod(input_parameters[parameterCounter]);    ++parameterCounter;// alpha in Equation ID: "contrastUpdate" of pdf
    const double nMaxOuter                            = stoi(input_parameters[parameterCounter]);    ++parameterCounter;
    const int    nMaxInner                            = stoi(input_parameters[parameterCounter]);    ++parameterCounter;
    randomInversionInput input
    {
       toleratnceOuter,                   nMaxOuter,       nMaxInner
    };
    return input;

}
