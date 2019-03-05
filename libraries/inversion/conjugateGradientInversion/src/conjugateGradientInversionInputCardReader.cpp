#include <iostream>

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
    std::string fileLocation = inputCardPath + "CGInput.json";
    std::ifstream in(fileLocation);

    if(!in.is_open())
    {
        std::cout << "Can't open file at " << fileLocation;
        exit(EXIT_FAILURE);
    }

    nlohmann::json j;
	in >> j;

    conjugateGradientInput input
	{
		{j["Iter1"]["n"], j["Iter1"]["tolerance"]},
        {j["DeltaAmplification"]["start"], j["DeltaAmplification"]["slope"]},
        j["do_reg"],
        j["n_max"]
	};

    _input = input;
}

