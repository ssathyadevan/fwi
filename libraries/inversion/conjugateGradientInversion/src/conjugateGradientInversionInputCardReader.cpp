#include "conjugateGradientInversionInputCardReader.h"
conjugateGradientInversionInputCardReader::conjugateGradientInversionInputCardReader(std::string inputCardPath, std::string outputLocation, std::string cardName): inputCardReader()
{
    conjugateGradientInput mInput = readCard(inputCardPath,outputLocation,cardName);
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
    vector<string> input_parameters = reader(inputCardPath+cardName+".in");
    int parameterCounter=0;
    const double tol1_to_be_implemented               = stod(input_parameters[parameterCounter]);    ++parameterCounter; //
    const double delta_amplification_start            = stod(input_parameters[parameterCounter]);    ++parameterCounter; //
    const double delta_amplification_slope            = stod(input_parameters[parameterCounter]);    ++parameterCounter; //
    const int    n_max                                = stoi(input_parameters[parameterCounter]);    ++parameterCounter;
    const int    n_iter1                              = stoi(input_parameters[parameterCounter]);    ++parameterCounter;
    if (!is_this_our_kind_of_bool(input_parameters[parameterCounter])){exit(EXIT_FAILURE);}
    const bool   do_reg                               = string_1_for_true_0_for_false(input_parameters[parameterCounter]); ++ parameterCounter;
    conjugateGradientInput input
    {
        n_iter1,                   tol1_to_be_implemented,
        delta_amplification_start, delta_amplification_slope,
        do_reg;
        n_max;
    };
    // This part is needed for plotting the chi values in imageCreator_CMake.py
    std::ofstream outputfwi;
    outputfwi.open(outputLocation + cardName + ".pythonIn");
    outputfwi << "This run was parametrized as follows:" << std::endl;
    outputfwi << "nxt   = " << input.ngrid[0] << std::endl;
    outputfwi << "nzt   = " << input.ngrid[1] << std::endl;
    outputfwi << "nMax = " << input.n_max    << std::endl;
    outputfwi << "iter1 = " << input.iter1.n  << std::endl;
    outputfwi << "iter2 = " << input.iter2.n  << std::endl;
    outputfwi.close();
    // This part is needed for plotting the chi values in imageCreator_CMake.py
    std::ofstream lastrun;
    lastrun.open(outputLocation + "lastRunName.txt");
    lastrun << cardName;
    lastrun.close();

    return input;
}
}
