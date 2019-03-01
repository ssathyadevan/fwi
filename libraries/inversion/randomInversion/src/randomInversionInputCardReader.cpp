#include "randomInversionInputCardReader.h"

randomInversionInputCardReader::randomInversionInputCardReader(std::string inputCardPath)
    :inputCardReader()
{
    readCard(inputCardPath);
}

randomInversionInput randomInversionInputCardReader::getInput()
{
    return _input;
}


void randomInversionInputCardReader::readCard(std::string inputCardPath)
{
    std::string filePath = inputCardPath + "RandomInversionInput.in";
    std::vector<std::string> input_parameters = readFile(filePath);

    int parameterCounter = 0;

    const double toleranceOuter                       = stod(input_parameters[parameterCounter]);    ++parameterCounter; // alpha in Equation ID: "contrastUpdate" of pdf
    const int    nMaxOuter                            = stoi(input_parameters[parameterCounter]);    ++parameterCounter;
    const int    nMaxInner                            = stoi(input_parameters[parameterCounter]);    ++parameterCounter;

    randomInversionInput input
    {
       toleranceOuter,         nMaxOuter,       nMaxInner
    };

    _input = input;
}
