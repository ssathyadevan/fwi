#include "conjugateGradientInversionInputCardReader.h"

conjugateGradientInversionInputCardReader::conjugateGradientInversionInputCardReader(std::string inputCardPath, std::string outputLocation, std::string cardName): inputCardReader()
{
    readCard(inputCardPath,outputLocation,cardName);
}

conjugateGradientInput  conjugateGradientInversionInputCardReader::getInput(){
    return _input;
}


void conjugateGradientInversionInputCardReader::readCard(std::string inputCardPath, std::string outputLocation, std::string cardName)
{
    std::vector<std::string> input_parameters = Reader(inputCardPath+cardName+".in");
    int parameterCounter=0;
    const double tol1_to_be_implemented               = stod(input_parameters[parameterCounter]);    ++parameterCounter; //
    const double delta_amplification_start            = stod(input_parameters[parameterCounter]);    ++parameterCounter; //
    const double delta_amplification_slope            = stod(input_parameters[parameterCounter]);    ++parameterCounter; //
    const int    n_max                                = stoi(input_parameters[parameterCounter]);    ++parameterCounter;
    const int    n_iter1                              = stoi(input_parameters[parameterCounter]);    ++parameterCounter;
    if (!CheckBoolString(input_parameters[parameterCounter])){exit(EXIT_FAILURE);}
    const bool   do_reg                               = InputStringToBool(input_parameters[parameterCounter]); ++ parameterCounter;
    conjugateGradientInput input
    {
        {n_iter1,                   tol1_to_be_implemented},
        {delta_amplification_start, delta_amplification_slope},
        do_reg,
        n_max
    };


    _input = input;
}

