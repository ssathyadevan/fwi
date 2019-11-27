#include <iostream>

#include "MPIConjugateGradientInversionInputCardReader.h"
#include "json.h"

MPIConjugateGradientInversionInputCardReader::MPIConjugateGradientInversionInputCardReader(const std::string &caseFolder)
    : InputCardReader()
{
    readCard(caseFolder);
}

MPIConjugateGradientInversionInput MPIConjugateGradientInversionInputCardReader::getInput()
{
    return _input;
}

void MPIConjugateGradientInversionInputCardReader::readCard(const std::string &caseFolder)
{
    nlohmann::json j = readFile(caseFolder + "/input/MPIConjugateGradientInversionInput.json");

    MPIConjugateGradientInversionInput input{
        {j["Iter1"]["n"], j["Iter1"]["tolerance"]},
        {j["DeltaAmplification"]["start"], j["DeltaAmplification"]["slope"]},
        j["do_reg"],
        j["n_max"]};

    _input = input;
}
