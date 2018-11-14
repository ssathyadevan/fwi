#include "grid_rect_2D.h"
#include "volField_rect_2D.h"
#include "ProfileCpu.h"
#include "sources_rect_2D.h"
#include "receivers_rect_2D.h"
#include "frequencies_group.h"
#include "inversion.h"
#include "InversionConcrete_cpu.h"
#include "read_input_fwi_into_vec.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <ios>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <string>
#include <ctime>

using std::cout;
using std::endl;

/*
Babak and Saurabh 2018-11-13:
Creating the pre-processing programm.
*/

const int g_verbosity = 0;
int generateRefrencePressureFieldFromChi (const std::string &fileName, const int& nxt, const int& nzt, const int& nSrct,
                                          const int& nFreq_Total, const double& Freq_min, const double& Freq_max,
                                          const double (&reservoir_corner_points_in_m)[2][2], const double& c_0,
                                          double tol2_to_be_implemented, bool calc_alpha, int n_iter2, std::string runName,
					  std::array<int,2>);
//Here we determine (is_this_our_kind_of_bool) if the reader correctly gave us a Boolean (1 or 0)
//and then set the Boolean from the input string (string_1_for_true_0_for_false)
bool is_this_our_kind_of_bool(std::string const& string_for_bool)
{return (string_for_bool.size() == 1 && (string_for_bool[0] == '0' || string_for_bool[0] == '1'));}

bool string_1_for_true_0_for_false(std::string const& string_for_bool)
{return string_for_bool[0] == '1';}

int main(int argc, char** argv)
{
    std::string runName, findPath;
    if (argc != 2)
    {
        runName = "default";
	findPath = "../";

    }
    else
    {
        runName = argv[1];
	findPath = "../InputOutput/";
    }
    // if g_verbosity is 0, print everything to a file program_output.txt, else if g_verbosity = 10, print everything on screen
    if (g_verbosity == 0)
    {
        std::cout << "Printing the program output onto a file named: "+runName+".out in the inputOutput folder" << std::endl;
        std::string tempString = "../inputOutput/" + runName + ".out";
        if (freopen(tempString.c_str(),"w", stdout)) {}
    }

    std::vector<std::string> input_parameters = reader(findPath + runName + ".in");

    struct DeltaAmplification{
        double start; double slope;
    };
    struct Freq{
        double min;   double max;         int nTotal;
    };
    struct Grid{
        int xSize;    int ySize;
    };
    struct ConjGrad{
        int nIter;    double tolerance;   bool calcAlpha;
    };
    struct Iter1{
        int n;        double tolerance;
    };
    struct NSourcesReceivers{
        int Src;      int Rec;
    };
    struct Input{
        double c_0;
        int n_max;
        bool do_reg;
        DeltaAmplification deltaAmplification;
        Freq freq;
        Grid grid;
        ConjGrad conjGrad;
        Iter1 iter1;
        NSourcesReceivers nSourcesReceivers;
        std::array<int,2> ngrid;//NEW = {nxt, nzt};
        //double reservoirCornerPointsInM[2][2]
    };

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
    if (!is_this_our_kind_of_bool(input_parameters[parameterCounter])){return 1;}
    const bool      calc_alpha                        = string_1_for_true_0_for_false(input_parameters[parameterCounter]); ++ parameterCounter; // alpha in Equation ID: "contrastUpdate" of pdf
    const double    tol1_to_be_implemented            = stod(input_parameters[parameterCounter]);    ++parameterCounter; //
    const double    tol2_to_be_implemented            = stod(input_parameters[parameterCounter]);    ++parameterCounter; //
    const double    delta_amplification_start         = stod(input_parameters[parameterCounter]);    ++parameterCounter; //
    const double    delta_amplification_slope         = stod(input_parameters[parameterCounter]);    ++parameterCounter; //
    const int       n_max                             = stoi(input_parameters[parameterCounter]);    ++parameterCounter;
    const int       n_iter1                           = stoi(input_parameters[parameterCounter]);    ++parameterCounter;
    const int       n_iter2                           = stoi(input_parameters[parameterCounter]);    ++parameterCounter;
    if (!is_this_our_kind_of_bool(input_parameters[parameterCounter])){return 1;}
    const bool      do_reg                            = string_1_for_true_0_for_false(input_parameters[parameterCounter]); ++ parameterCounter;

    //We transfer what is in input_parameters to the relevant constants
    const double reservoir_corner_points_in_m[2][2] = {{top_left_corner_coord_x_in_m,top_left_corner_coord_z_in_m},{bottom_right_corner_coord_x_in_m,bottom_right_corner_coord_z_in_m}};

    Input input{//NEW not a const now to keep things simple
        c_0, n_max, do_reg,
        delta_amplification_start, delta_amplification_slope,
        Freq_min,                  Freq_max,                     nFreq_Total,
        nxt,                       nzt,
        n_iter2,                   tol2_to_be_implemented,       calc_alpha,
        n_iter1,                   tol1_to_be_implemented,
        nSrct,                     nRecv,
        {nxt, nzt} 
    };

    // This part is needed for plotting the chi values in imageCreator_CMake.py
    std::ofstream outputfwi;
    outputfwi.open("../inputOutput/" + runName + ".pythonIn");
    outputfwi << "This run was parametrized as follows:" << std::endl;
    outputfwi << "nxt   = " << nxt << std::endl;
    outputfwi << "nzt   = " << nzt << std::endl;
    outputfwi.close();
    // This part is needed for plotting the chi values in imageCreator_CMake.py
    std::ofstream lastrun;
    lastrun.open("../inputOutput/lastRunName.txt");
    lastrun << runName;
    lastrun.close();

    cout << "Preprocessing the provided input to create the reference pressure-field" << endl;
    std::time_t start = std::time(nullptr);
    std::cout << "Starting at " <<  std::asctime(std::localtime(&start)) << std::endl;
    std::cout << "nFreq_Total= " << nFreq_Total << std::endl;
    // Generating the reference pressure field from input data (chi values)
    int ret =generateRefrencePressureFieldFromChi(fileName, nxt, nzt, nSrct, nFreq_Total, Freq_min, Freq_max,
                                                  reservoir_corner_points_in_m, c_0, calc_alpha, n_iter2, do_reg, runName, input.ngrid);
    std::cout << ret << std::endl;
    std::time_t finish = std::time(nullptr);
    std::cout << "Finished at " <<  std::asctime(std::localtime(&finish)) << std::endl;
    return 0;
}

int generateRefrencePressureFieldFromChi (const std::string &fileName, const int& nxt, const int& nzt, const int& nSrct,
                                          const int& nFreq_Total, const double& Freq_min, const double& Freq_max,
                                          const double (&reservoir_corner_points_in_m)[2][2], const double& c_0, double tol2_to_be_implemented,
                                          bool calc_alpha, int n_iter2, std::string runName, std::array<int,2> ngrid2)
{
    std::array<double,2> x_min = {(reservoir_corner_points_in_m[0][0]), (reservoir_corner_points_in_m[0][1])};
    std::array<double,2> x_max = {(reservoir_corner_points_in_m[1][0]), (reservoir_corner_points_in_m[1][1])};
    std::array<int,2> ngrid = ngrid2;//{nxt, nzt};NEW

    grid_rect_2D grid(x_min, x_max, ngrid);
    volField_rect_2D_cpu chi(grid);

    chi.fromFile(fileName);

    std::array<double,2> x_src_min = {-480.0, -5.0};
    std::array<double,2> x_src_max = {480.0, -5.0};

    Sources_rect_2D src(x_src_min, x_src_max, nSrct);
    src.Print();

    int nRecv = nSrct;
    Receivers_rect_2D recv(src);
    recv.Print();

    double f_min, d_freq_proc;

    double d_freq = (Freq_max - Freq_min)/(nFreq_Total -1);

    int sum = 0;


    f_min = Freq_min + sum*d_freq;
    d_freq_proc = d_freq;

    Frequencies_group freq(f_min, d_freq_proc, nFreq_Total, c_0);
    freq.Print();

    ProfileInterface *profiler;
    profiler = new ProfileCpu();

    std::complex<double> referencePressureData[nFreq_Total * nRecv * nSrct];

    chi.toFile("../inputOutput/chi_ref_" + runName + ".txt");

    Inversion *inverse;
    inverse = new InversionConcrete_cpu(grid, src, recv, freq, *profiler);

    std::cout << "Creating Greens function field..." << std::endl;
    inverse->createGreens();
    inverse->SetBackground(chi);

    std::cout << "Creating P0..." << std::endl;
    inverse->createP0();

    std::cout << "Creating total field..." << std::endl;
    inverse->createTotalField(tol2_to_be_implemented, calc_alpha, n_iter2);

    std::cout << "Calculate pData (the reference pressure-field)..." << std::endl;
    inverse->calculateData(referencePressureData);
    // writing the referencePressureData to a text file in complex form
    std::string invertedChiToPressureFileName = "../inputOutput/invertedChiToPressure.txt";
    std::ofstream file;
    file.open (invertedChiToPressureFileName, std::ios::out | std::ios::trunc);
    assert(file.is_open());
    file << std::setprecision(17) << "# This is the file which contains the reference pressure"
                                     " data obtained from the reference chi values in the form "
                                     "of complex numbers." << std::endl;
    file << std::setprecision(17) << "# First column indicates the real values and the second "
                                     "column indicates the imaginary values." << std::endl;
    for(int i=0; i < nFreq_Total * nRecv * nSrct; i++) {
        file << std::setprecision(17) << referencePressureData[i].real() <<"," << referencePressureData[i].imag() << std::endl;
    }
    file.close();
    return 0;
}
