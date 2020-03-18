#include <iostream>

#include "conjugateGradientInversionInputCardReader.h"
#include "json.h"

ConjugateGradientInversionInputCardReader::ConjugateGradientInversionInputCardReader(const std::string &caseFolder)
    : InputCardReader()
{
    readCard(caseFolder);
    checkinput();
}

ConjugateGradientInversionInput ConjugateGradientInversionInputCardReader::getInput()
{
    return _input;
}

void ConjugateGradientInversionInputCardReader::readCard(const std::string &caseFolder)
{
    nlohmann::json j = readFile(caseFolder + "/input/ConjugateGradientInversionInput.json");

    ConjugateGradientInversionInput input{
        {j["Iter1"]["n"], j["Iter1"]["tolerance"]},
        {j["DeltaAmplification"]["start"], j["DeltaAmplification"]["slope"]},
        j["do_reg"],
        j["n_max"]};

    _input = input;
}

void ConjugateGradientInversionInputCardReader::checkinput()
{
    if (_input.iteration1.n <= 0) {throw std::invalid_argument("Invalid number of iterations n in ConjugateGradientInversionInput.json");}
    if (_input.iteration1.tolerance <= 0) {throw std::invalid_argument("Invalid tolerance in ConjugateGradientInversionInput.json");}
    if (_input.n_max <= 0 ) {throw std::invalid_argument("Invalid number of iterations n_max in ConjugateGradientInversionInput.json");}
}
