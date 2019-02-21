#include "conjugateGradientInversionInputCardReader.h"

conjugateGradientInversionInputCardReader::conjugateGradientInversionInputCardReader(std::string inputCardPath, std::string outputLocation, std::string cardName): inputCardReader()
{
    mInput = readCard(inputCardPath,outputLocation,cardName);
}

conjugateGradientInput  conjugateGradientInversionInputCardReader::getInput(){
    return mInput;
}

std::vector<std::string> conjugateGradientInversionInputCardReader::Reader(const std::string& runName) const
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

conjugateGradientInput conjugateGradientInversionInputCardReader::readCard(std::string inputCardPath, std::string outputLocation, std::string cardName)
{
    std::vector<std::string> input_parameters = Reader(inputCardPath+cardName+".in");
    int parameterCounter=0;
    const double tol1_to_be_implemented               = stod(input_parameters[parameterCounter]);    ++parameterCounter; //
    const double delta_amplification_start            = stod(input_parameters[parameterCounter]);    ++parameterCounter; //
    const double delta_amplification_slope            = stod(input_parameters[parameterCounter]);    ++parameterCounter; //
    const int    n_max                                = stoi(input_parameters[parameterCounter]);    ++parameterCounter;
    const int    n_iter1                              = stoi(input_parameters[parameterCounter]);    ++parameterCounter;
    if (!IsThisOurKindOfBool(input_parameters[parameterCounter])){exit(EXIT_FAILURE);}
    const bool   do_reg                               = String1ForTrue0ForFalse(input_parameters[parameterCounter]); ++ parameterCounter;
    conjugateGradientInput input
    {
        {n_iter1,                   tol1_to_be_implemented},
        {delta_amplification_start, delta_amplification_slope},
        do_reg,
        n_max
    };


    return input;
}

