#include <iostream>

#include "MPIConjugateGradientInversionInputCardReader.h"
#include "json.h"

MPIConjugateGradientInversionInputCardReader::MPIConjugateGradientInversionInputCardReader(const std::string &caseFolder)
    : InputCardReader()
{
    readCard(caseFolder);
    checkinput();
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

void MPIConjugateGradientInversionInputCardReader::checkinput()
{
    if (_input.iteration1.n <= 0) {throw std::invalid_argument("Invalid number of iterations n in MPIConjugateGradientInversionInput.json");}
    if (_input.iteration1.tolerance <= 0) {throw std::invalid_argument("Invalid tolerance in MPIConjugateGradientInversionInput.json");}
    if (_input.n_max <= 0 ) {throw std::invalid_argument("Invalid number of iterations n_max in MPIConjugateGradientInversionInput.json");}v
}
