#include "forwardModelInputCardReader.h"
#include "json.h"

forwardModelInputCardReader::forwardModelInputCardReader(std::string inputCardPath)
    : inputCardReader()
{
   readCard(inputCardPath);
}

forwardModelInput forwardModelInputCardReader::getInput()
{
    return _input;
}

void forwardModelInputCardReader::readCard(std::string inputCardPath)
{
    std::ifstream in(inputCardPath + "FMInput.json");
    nlohmann::json j;
    in >> j;

    forwardModelInput jsonInput
    {
        j["Iter2"]["n"], j["Iter2"]["tolerance"], j["Iter2"]["calcAlpha"]
    };

//    std::string filePath = inputCardPath + "ForwardModelInput.in";
//    std::vector<std::string> input_parameters = readFile(filePath);

//    int parameterCounter = 0;

//    const bool   calc_alpha                 = InputStringToBool(input_parameters[parameterCounter]); ++parameterCounter; // alpha in Equation ID: "contrastUpdate" of pdf
//    const double tol2_to_be_implemented     = stod(input_parameters[parameterCounter]);              ++parameterCounter;
//    const int    n_iter2                    = stoi(input_parameters[parameterCounter]);              ++parameterCounter;

//    forwardModelInput input
//    {
//       n_iter2,                   tol2_to_be_implemented,       calc_alpha
//    };

//    if(input.iter2.tolerance != jsonInput.iter2.tolerance)
//        std::cout << "file read wrong" << std::endl;
//    //_input = input;
    _input = jsonInput;
}
