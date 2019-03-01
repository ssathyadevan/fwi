#include "conjugateGradientInversionInputCardReader.h"
#include "json.h"

conjugateGradientInversionInputCardReader::conjugateGradientInversionInputCardReader(std::string inputCardPath)
    : inputCardReader()
{
    readCard(inputCardPath);
}

conjugateGradientInput  conjugateGradientInversionInputCardReader::getInput(){
    return _input;
}

void conjugateGradientInversionInputCardReader::readCard(std::string inputCardPath)
{
	std::ifstream in(intputCardPath + "CGInput.json");
	nlohmann::json j;
	in >> j;
	
	conjugateGradientInput jsonInput
	{
		{j["Iter1"]["n"], j["Iter1"]["tolerance"]},
        {j["DeltaAmplification"]["start"], j["DeltaAmplification"]["slope"]},
        j["do_reg"],
        j["n_max"]
	};
	
    std::vector<std::string> input_parameters = readFile(inputCardPath + "ConjugateGradientInversionInput.in");
    int parameterCounter = 0;

    const double tol1_to_be_implemented               = stod(input_parameters[parameterCounter]);    ++parameterCounter; //
    const double delta_amplification_start            = stod(input_parameters[parameterCounter]);    ++parameterCounter; //
    const double delta_amplification_slope            = stod(input_parameters[parameterCounter]);    ++parameterCounter; //
    const int    n_max                                = stoi(input_parameters[parameterCounter]);    ++parameterCounter;
    const int    n_iter1                              = stoi(input_parameters[parameterCounter]);    ++parameterCounter;
    const bool   do_reg                               = InputStringToBool(input_parameters[parameterCounter]); ++ parameterCounter;

    conjugateGradientInput input
    {
        {n_iter1,                   tol1_to_be_implemented},
        {delta_amplification_start, delta_amplification_slope},
        do_reg,
        n_max
    };

    //_input = input;
    _input = jsonInput;
}

