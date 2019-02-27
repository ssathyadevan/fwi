#include "genericInputCardReader.h"
#include <iostream>

genericInputCardReader::genericInputCardReader(std::string inputCardPath, std::string outputLocation, std::string cardName): inputCardReader()
{
    readCard(inputCardPath,outputLocation,cardName);
}

genericInput genericInputCardReader::getInput()
{
    return _input;
}

void genericInputCardReader::readCard(std::string inputCardPath, std::string outputLocation, std::string cardName){
    std::string filePath = inputCardPath+cardName+".in";
    std::vector<std::string> input_parameters = readFile(filePath);

    int parameterCounter = 0;

    const double c_0                                  = stod(input_parameters[parameterCounter]);    ++parameterCounter; //Speed of sound in background
    const double Freq_min                             = stod(input_parameters[parameterCounter]);    ++parameterCounter; //Minimum frequency
    const double Freq_max                             = stod(input_parameters[parameterCounter]);    ++parameterCounter; //Maximum frequency
    const double top_left_corner_coord_x_in_m         = stod(input_parameters[parameterCounter]);    ++parameterCounter;
    const double top_left_corner_coord_z_in_m         = stod(input_parameters[parameterCounter]);    ++parameterCounter;
    const double bottom_right_corner_coord_x_in_m     = stod(input_parameters[parameterCounter]);    ++parameterCounter;
    const double bottom_right_corner_coord_z_in_m     = stod(input_parameters[parameterCounter]);    ++parameterCounter;
    const double srcs_top_left_corner_coord_x_in_m    = stod(input_parameters[parameterCounter]);    ++parameterCounter;
    const double srcs_top_left_corner_coord_z_in_m    = stod(input_parameters[parameterCounter]);    ++parameterCounter;
    const double srcs_bottom_right_corner_coord_x_in_m= stod(input_parameters[parameterCounter]);    ++parameterCounter;
    const double srcs_bottom_right_corner_coord_z_in_m= stod(input_parameters[parameterCounter]);    ++parameterCounter;
    const int    nxt                                  = stoi(input_parameters[parameterCounter]);    ++parameterCounter; //Number of grid points horizontal
    const int    nzt                                  = stoi(input_parameters[parameterCounter]);    ++parameterCounter; //Number of grid points vertical
    const int    nFreq_Total                          = stoi(input_parameters[parameterCounter]);    ++parameterCounter; //Total number of frequencies used
    const int    nSrct                                = stoi(input_parameters[parameterCounter]);    ++parameterCounter; //Number of sources
    const int    nRecv                                = stoi(input_parameters[parameterCounter]);    ++parameterCounter; //Number of sources
    const std::string  fileName                       =     (input_parameters[parameterCounter]);    ++parameterCounter; //Filename to be used for inversion
    const bool   verbose                              = InputStringToBool(input_parameters[parameterCounter]); ++ parameterCounter;
    const double spacing = ((Freq_max-Freq_min)/(nFreq_Total-1));

    genericInput input
    {
                inputCardPath, outputLocation, cardName,
                c_0,
                {Freq_min,                  Freq_max,                     nFreq_Total,     spacing},
                {top_left_corner_coord_x_in_m, top_left_corner_coord_z_in_m},
                {bottom_right_corner_coord_x_in_m, bottom_right_corner_coord_z_in_m},
                {srcs_top_left_corner_coord_x_in_m, srcs_top_left_corner_coord_z_in_m},
                {srcs_bottom_right_corner_coord_x_in_m, srcs_bottom_right_corner_coord_z_in_m},
                {nxt, nzt},
                {nSrct, nRecv},
                fileName,
                verbose
     };

    _input = input;
}
