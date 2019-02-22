#include "forwardModelInputCardReader.h"

forwardModelInputCardReader::forwardModelInputCardReader(std::string inputCardPath, std::string outputLocation, std::string cardName): inputCardReader()
{
    mInput = readCard(inputCardPath,outputLocation,cardName);
}

forwardModelInput forwardModelInputCardReader::getInput(){
    return mInput;
}

std::vector<std::string> forwardModelInputCardReader::Reader(const std::string& runName) const
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
forwardModelInput forwardModelInputCardReader::readCard(std::string inputCardPath, std::string outputLocation, std::string cardName)
{
    std::string filePath = inputCardPath+cardName+".in";
    std::vector<std::string> input_parameters = Reader(filePath);
    int parameterCounter=0;
    if (!CheckBoolString(input_parameters[parameterCounter])){exit(EXIT_FAILURE);}
    const bool   calc_alpha                           = InputStringToBool(input_parameters[parameterCounter]); ++ parameterCounter; // alpha in Equation ID: "contrastUpdate" of pdf
    const double tol2_to_be_implemented               = stod(input_parameters[parameterCounter]);    ++parameterCounter;
    const int    n_iter2                              = stoi(input_parameters[parameterCounter]);    ++parameterCounter;
    forwardModelInput input
    {
       n_iter2,                   tol2_to_be_implemented,       calc_alpha
    };
    return input;

}
