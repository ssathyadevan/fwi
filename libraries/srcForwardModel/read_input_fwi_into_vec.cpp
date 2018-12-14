#include "read_input_fwi_into_vec.h"

//Here we determine (is_this_our_kind_of_bool) if the reader correctly gave us a Boolean (1 or 0)
//and then set the Boolean from the input string (string_1_for_true_0_for_false)

bool is_this_our_kind_of_bool(std::string const& string_for_bool)
{return (string_for_bool.size() == 1 && (string_for_bool[0] == '0' || string_for_bool[0] == '1'));}

bool string_1_for_true_0_for_false(std::string const& string_for_bool)
{return string_for_bool[0] == '1';}

vector<string> reader(std::string runName)
{
    vector<string> thevec;
    fstream f1(runName);
    string line,w1;
    while ( getline(f1,line) )
    {
        if (( istringstream(line) >> w1)  && ( w1[0] != '#' ))
        {
            thevec.push_back(w1);
        }
    }
    return thevec;
}

struct Input reader3(int argc, char** argv)
{
    std::string inputCardPath;
    std::string outputLocation;
    std::string cardName;

    if (argc != 4)
    {
        std::cout<< "Please enter 3 arguments, 1st the input card path, 2nd the output folder location and 3rd the input card name" << std::endl;
        std::cout<< "e.g. ./FWI_Process ~/Documents/FWIInstall/ ~/Documents/FWIInstall/Output/ cardName" << std::endl;

        exit(EXIT_FAILURE);
    }
    else
    {
        inputCardPath = argv[1];
        outputLocation = argv[2];
        cardName = argv[3];
    }

    vector<string> input_parameters = reader(inputCardPath+cardName+".in");
    int parameterCounter=0;
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
    if (!is_this_our_kind_of_bool(input_parameters[parameterCounter])){exit(EXIT_FAILURE);}
    const bool   calc_alpha                           = string_1_for_true_0_for_false(input_parameters[parameterCounter]); ++ parameterCounter; // alpha in Equation ID: "contrastUpdate" of pdf
    const double tol1_to_be_implemented               = stod(input_parameters[parameterCounter]);    ++parameterCounter; //
    const double tol2_to_be_implemented               = stod(input_parameters[parameterCounter]);    ++parameterCounter; //
    const double delta_amplification_start            = stod(input_parameters[parameterCounter]);    ++parameterCounter; //
    const double delta_amplification_slope            = stod(input_parameters[parameterCounter]);    ++parameterCounter; //
    const int    n_max                                = stoi(input_parameters[parameterCounter]);    ++parameterCounter;
    const int    n_iter1                              = stoi(input_parameters[parameterCounter]);    ++parameterCounter;
    const int    n_iter2                              = stoi(input_parameters[parameterCounter]);    ++parameterCounter;
    if (!is_this_our_kind_of_bool(input_parameters[parameterCounter])){exit(EXIT_FAILURE);}
    const bool   do_reg                               = string_1_for_true_0_for_false(input_parameters[parameterCounter]); ++ parameterCounter;
    if (!is_this_our_kind_of_bool(input_parameters[parameterCounter])){exit(EXIT_FAILURE);}
    const bool   verbose                              = string_1_for_true_0_for_false(input_parameters[parameterCounter]); ++ parameterCounter;

    const double spacing = ((Freq_max-Freq_min)/(nFreq_Total-1));

    Input input{//NEW not a const now to keep things simple
        inputCardPath, outputLocation, cardName,
                c_0, n_max, do_reg, verbose,
                delta_amplification_start, delta_amplification_slope,
                Freq_min,                  Freq_max,                     nFreq_Total,     spacing ,
                n_iter2,                   tol2_to_be_implemented,       calc_alpha,
                n_iter1,                   tol1_to_be_implemented,
                nSrct,                     nRecv,
        {nxt, nzt},
        fileName,
        {top_left_corner_coord_x_in_m,top_left_corner_coord_z_in_m},
        {bottom_right_corner_coord_x_in_m,bottom_right_corner_coord_z_in_m},
        {srcs_top_left_corner_coord_x_in_m,srcs_top_left_corner_coord_z_in_m},
        {srcs_bottom_right_corner_coord_x_in_m,srcs_bottom_right_corner_coord_z_in_m}
    };

    // This part is needed for plotting the chi values in imageCreator_CMake.py
    std::ofstream outputfwi;
    outputfwi.open(outputLocation + cardName + ".pythonIn");
    outputfwi << "This run was parametrized as follows:" << std::endl;
    outputfwi << "nxt   = " << input.ngrid[0] << std::endl;
    outputfwi << "nzt   = " << input.ngrid[1] << std::endl;
    outputfwi << "nMax = " << input.n_max    << std::endl;
    outputfwi << "iter1 = " << input.iter1.n  << std::endl;
    outputfwi << "iter2 = " << input.iter2.n  << std::endl;
    outputfwi.close();
    // This part is needed for plotting the chi values in imageCreator_CMake.py
    std::ofstream lastrun;
    lastrun.open(outputLocation + "lastRunName.txt");
    lastrun << cardName;
    lastrun.close();

    return input;
}
