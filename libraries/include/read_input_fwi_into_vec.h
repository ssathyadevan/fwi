#ifndef READERVAR
#define READERVAR

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <ios>
#include <vector>


using namespace std;
  
    array<string, 2> InputPathNName(int argc, char** argv){
	array<string, 2> inLocNName;
        if (argc != 2)
	{
	    inLocNName[0] = "../";
            inLocNName[1] = "default";
	}
	else
	{
	    inLocNName[0] = "../inputOutput/";//argv[1];
	    inLocNName[1] = argv[1];//"../InputOutput/";
	}
	return inLocNName;
    }
   
    struct DeltaAmplification{
        double start; double slope;
    };
    struct Freq{
        double min;   double max;         int nTotal;
    };
    /*struct Grid{
        int xSize;    int ySize;
    };*/
    struct ConjGrad{
        int nIter;    double tolerance;   bool calcAlpha;
    };
    struct Iter1{
        int n;        double tolerance;
    };
    struct NSourcesReceivers{
        int src;      int rec;
    };
    struct Input{
        double c_0;
        int n_max;
        bool do_reg;
	bool verbose;
        DeltaAmplification deltaAmplification;
        Freq freq;
        //Grid grid;
        ConjGrad conjGrad;
        Iter1 iter1;
        NSourcesReceivers nSourcesReceivers;
        std::array<int,2> ngrid;//NEW = {nxt, nzt};
	std::string fileName;
        double reservoirCornerPointsInM[2][2];
    };

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

struct Input reader2(std::string runName)
{
    vector<string> input_parameters = reader(runName);
    int parameterCounter=0;
    const double    c_0                               = stod(input_parameters[parameterCounter]);    ++parameterCounter; //Speed of sound in background
    const double    Freq_min                          = stod(input_parameters[parameterCounter]);    ++parameterCounter; //Minimum frequency
    const double    Freq_max                          = stod(input_parameters[parameterCounter]);    ++parameterCounter; //Maximum frequency
    const double    top_left_corner_coord_x_in_m      = stod(input_parameters[parameterCounter]);    ++parameterCounter;
    const double    top_left_corner_coord_z_in_m      = stod(input_parameters[parameterCounter]);    ++parameterCounter;
    const double    bottom_right_corner_coord_x_in_m  = stod(input_parameters[parameterCounter]);    ++parameterCounter;
    const double    bottom_right_corner_coord_z_in_m  = stod(input_parameters[parameterCounter]);    ++parameterCounter;
    const int       nxt                               = stoi(input_parameters[parameterCounter]);    ++parameterCounter; //Number of grid points horizontal
    const int       nzt                               = stoi(input_parameters[parameterCounter]);    ++parameterCounter; //Number of grid points vertical
    const int       nFreq_Total                       = stoi(input_parameters[parameterCounter]);    ++parameterCounter; //Total number of frequencies used
    const int       nSrct                             = stoi(input_parameters[parameterCounter]);    ++parameterCounter; //Number of sources
    const int       nRecv                             = stoi(input_parameters[parameterCounter]);    ++parameterCounter; //Number of sources
    const std::string  fileName                       =     (input_parameters[parameterCounter]);    ++parameterCounter; //Filename to be used for inversion
    if (!is_this_our_kind_of_bool(input_parameters[parameterCounter])){exit(EXIT_FAILURE);}
    const bool      calc_alpha                        = string_1_for_true_0_for_false(input_parameters[parameterCounter]); ++ parameterCounter; // alpha in Equation ID: "contrastUpdate" of pdf
    const double    tol1_to_be_implemented            = stod(input_parameters[parameterCounter]);    ++parameterCounter; //
    const double    tol2_to_be_implemented            = stod(input_parameters[parameterCounter]);    ++parameterCounter; //
    const double    delta_amplification_start         = stod(input_parameters[parameterCounter]);    ++parameterCounter; //
    const double    delta_amplification_slope         = stod(input_parameters[parameterCounter]);    ++parameterCounter; //
    const int       n_max                             = stoi(input_parameters[parameterCounter]);    ++parameterCounter;
    const int       n_iter1                           = stoi(input_parameters[parameterCounter]);    ++parameterCounter;
    const int       n_iter2                           = stoi(input_parameters[parameterCounter]);    ++parameterCounter;
    if (!is_this_our_kind_of_bool(input_parameters[parameterCounter])){exit(EXIT_FAILURE);}
    const bool      do_reg                            = string_1_for_true_0_for_false(input_parameters[parameterCounter]); ++ parameterCounter;
    if (!is_this_our_kind_of_bool(input_parameters[parameterCounter])){exit(EXIT_FAILURE);}
    const bool      verbose                           = string_1_for_true_0_for_false(input_parameters[parameterCounter]); ++ parameterCounter;

    Input input{//NEW not a const now to keep things simple
        c_0, n_max, do_reg, verbose,
        delta_amplification_start, delta_amplification_slope,
        Freq_min,                  Freq_max,                     nFreq_Total,
        //nxt,                       nzt,
        n_iter2,                   tol2_to_be_implemented,       calc_alpha,
        n_iter1,                   tol1_to_be_implemented,
        nSrct,                     nRecv,
        {nxt, nzt},
        fileName,
        {{top_left_corner_coord_x_in_m,top_left_corner_coord_z_in_m},{bottom_right_corner_coord_x_in_m,bottom_right_corner_coord_z_in_m}}
    };
    
    return input;

}
#endif
