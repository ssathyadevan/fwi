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
    std::ifstream in(inputCardPath + "CGInput.json");
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

