#include <iostream>

#include "conjugateGradientInversionInputCardReader.h"
#include "json.h"

conjugateGradientInversionInputCardReader::conjugateGradientInversionInputCardReader(const std::string &caseFolder)
    : inputCardReader()
{
    readCard(caseFolder);
}

conjugateGradientInversionInput  conjugateGradientInversionInputCardReader::getInput(){
    return _input;
}

void conjugateGradientInversionInputCardReader::readCard(const std::string &caseFolder)
{
    nlohmann::json j = readFile(caseFolder + "/input/CGInput.json");

    conjugateGradientInversionInput input
	{
		{j["Iter1"]["n"], j["Iter1"]["tolerance"]},
        {j["DeltaAmplification"]["start"], j["DeltaAmplification"]["slope"]},
        j["do_reg"],
        j["n_max"]
	};

    _input = input;
}

