#include "genericInputCardReader.h"
#include <iostream>
#include "json.h"

genericInputCardReader::genericInputCardReader(std::string inputCardPath, std::string outputLocation, std::string runName): inputCardReader()
{
    readCard(inputCardPath);
    _input.outputLocation = outputLocation;
    _input.runName = runName;
}

genericInput genericInputCardReader::getInput()
{
    return _input;
}

void genericInputCardReader::readCard(std::string inputCardPath){
    std::ifstream in(inputCardPath + "genericInput.json");
    nlohmann::json j;
    in >> j;

    double fMax = j["Freq"]["max"];
    double fMin = j["Freq"]["min"];
    int nFreq = j["Freq"]["nTotal"];
    const double spacing = (fMax - fMin)/(nFreq-1);

    std::cout << "blababababab " << j["c_0"] << std::endl;

    genericInput jsonInput
    {
        inputCardPath, "", "",
        j["c_0"],
        {j["Freq"]["min"], j["Freq"]["max"], j["Freq"]["nTotal"], spacing},
        {j["reservoirTopLeft"]["x"], j["reservoirTopLeft"]["z"]},
        {j["reservoirBottomRight"]["x"], j["reservoirBottomRight"]["z"]},
        {j["sourcesTopLeft"]["x"], j["sourcesTopLeft"]["z"]},
        {j["sourcesBottomRight"]["x"], j["sourcesBottomRight"]["z"]},
        {j["ngrid"]["x"], j["ngrid"]["z"]},
        {j["nSources"], j["nReceivers"]},
        j["fileName"],
        j["verbosity"]
    };

//    std::string filePath = inputCardPath + "GenericInput.in";
//    std::vector<std::string> input_parameters = readFile(filePath);

//    int parameterCounter = 0;

//    const double c_0                                  = stod(input_parameters[parameterCounter]);    ++parameterCounter; //Speed of sound in background
//    const double Freq_min                             = stod(input_parameters[parameterCounter]);    ++parameterCounter; //Minimum frequency
//    const double Freq_max                             = stod(input_parameters[parameterCounter]);    ++parameterCounter; //Maximum frequency
//    const double top_left_corner_coord_x_in_m         = stod(input_parameters[parameterCounter]);    ++parameterCounter;
//    const double top_left_corner_coord_z_in_m         = stod(input_parameters[parameterCounter]);    ++parameterCounter;
//    const double bottom_right_corner_coord_x_in_m     = stod(input_parameters[parameterCounter]);    ++parameterCounter;
//    const double bottom_right_corner_coord_z_in_m     = stod(input_parameters[parameterCounter]);    ++parameterCounter;
//    const double srcs_top_left_corner_coord_x_in_m    = stod(input_parameters[parameterCounter]);    ++parameterCounter;
//    const double srcs_top_left_corner_coord_z_in_m    = stod(input_parameters[parameterCounter]);    ++parameterCounter;
//    const double srcs_bottom_right_corner_coord_x_in_m= stod(input_parameters[parameterCounter]);    ++parameterCounter;
//    const double srcs_bottom_right_corner_coord_z_in_m= stod(input_parameters[parameterCounter]);    ++parameterCounter;
//    const int    nxt                                  = stoi(input_parameters[parameterCounter]);    ++parameterCounter; //Number of grid points horizontal
//    const int    nzt                                  = stoi(input_parameters[parameterCounter]);    ++parameterCounter; //Number of grid points vertical
//    const int    nFreq_Total                          = stoi(input_parameters[parameterCounter]);    ++parameterCounter; //Total number of frequencies used
//    const int    nSrct                                = stoi(input_parameters[parameterCounter]);    ++parameterCounter; //Number of sources
//    const int    nRecv                                = stoi(input_parameters[parameterCounter]);    ++parameterCounter; //Number of sources
//    const std::string  fileName                       =     (input_parameters[parameterCounter]);    ++parameterCounter; //Filename to be used for inversion
//    const bool   verbose                              = InputStringToBool(input_parameters[parameterCounter]); ++ parameterCounter;
//    const double spacing = ((Freq_max-Freq_min)/(nFreq_Total-1));

//    genericInput input
//    {
//                inputCardPath, "", "",
//                c_0,
//                {Freq_min,                  Freq_max,                     nFreq_Total,     spacing},
//                {top_left_corner_coord_x_in_m, top_left_corner_coord_z_in_m},
//                {bottom_right_corner_coord_x_in_m, bottom_right_corner_coord_z_in_m},
//                {srcs_top_left_corner_coord_x_in_m, srcs_top_left_corner_coord_z_in_m},
//                {srcs_bottom_right_corner_coord_x_in_m, srcs_bottom_right_corner_coord_z_in_m},
//                {nxt, nzt},
//                {nSrct, nRecv},
//                fileName,
//                verbose
//     };

//    _input = input;
    _input = jsonInput;
}
