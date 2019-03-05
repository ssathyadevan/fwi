#include "forwardModelInputCardReader.h"

forwardModelInputCardReader::forwardModelInputCardReader(std::string inputCardPath, std::string outputLocation, std::string cardName): inputCardReader()
{
   readCard(inputCardPath,outputLocation,cardName);
}

forwardModelInput forwardModelInputCardReader::getInput()
{
    return _input;
}

void forwardModelInputCardReader::readCard(std::string inputCardPath, std::string outputLocation, std::string cardName)
{
    std::string filePath = inputCardPath+cardName + ".in";
    std::vector<std::string> input_parameters = readFile(filePath);

    int parameterCounter = 0;

    const bool   calc_alpha                 = InputStringToBool(input_parameters[parameterCounter]); ++parameterCounter; // alpha in Equation ID: "contrastUpdate" of pdf
    const double tol2_to_be_implemented     = stod(input_parameters[parameterCounter]);              ++parameterCounter;
    const int    n_iter2                    = stoi(input_parameters[parameterCounter]);              ++parameterCounter;

    forwardModelInput input
    {
       n_iter2,                   tol2_to_be_implemented,       calc_alpha
    };

    _input = input;    
}
