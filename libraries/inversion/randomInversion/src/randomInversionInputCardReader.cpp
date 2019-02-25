#include "randomInversionInputCardReader.h"

randomInversionInputCardReader::randomInversionInputCardReader(std::string inputCardPath, std::string outputLocation, std::string cardName):inputCardReader()
{
    readCard(inputCardPath,outputLocation,cardName);
}

randomInversionInput randomInversionInputCardReader::getInput()
{
    return _input;
}


void randomInversionInputCardReader::readCard(std::string inputCardPath, std::string outputLocation, std::string cardName)
{
    std::string filePath = inputCardPath+cardName+".in";
    std::vector<std::string> input_parameters = readFile(filePath);

    int parameterCounter = 0;

    const double toleranceOuter                       = stod(input_parameters[parameterCounter]);    ++parameterCounter; // alpha in Equation ID: "contrastUpdate" of pdf
    const double nMaxOuter                            = stoi(input_parameters[parameterCounter]);    ++parameterCounter;
    const int    nMaxInner                            = stoi(input_parameters[parameterCounter]);    ++parameterCounter;

    randomInversionInput input
    {
       toleranceOuter,                   nMaxOuter,       nMaxInner
    };

    _input = input;
}
