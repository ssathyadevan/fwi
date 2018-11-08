#include <iostream>
#include <cmath>
#include <string>
#include <ctime>

#include "calcField.h"

#include "grid_rect_2D.h"

#include "volField_rect_2D.h"
#include "volComplexField_rect_2D_cpu.h"
#include "volComplexField_rect_2D.h"
#include "greens_rect_2D_cpu.h"
#include "ProfileCpu.h"

#include "sources_rect_2D.h"
#include "receivers_rect_2D.h"
#include "frequencies_group.h"

#include "inversion.h"
#include "inversion_cpu.h"
#include "helper.h"

//#include "tests_helper.h"
#include "input_parameters.h"
#include "frequencies_group.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ios>
#include <vector>
#include "read_input_fwi_into_vec.h"
#include "chi_visualisation_in_integer_form.h"
#include "create_csv_files_for_chi.h"
#include <stdexcept>

using std::cout;
using std::endl;

/* 
MELISSEN 2018-10-16
Introducing global variable (code base broad) g_ (global) verbosity
It controls the couts, because the couts in the original code are time consuming.
0 means silent (production mode: fast), 10 means verbose (diagnosis mode: slow)
In the future we can add more flavors of verbosity.
*/
const int g_verbosity = 0;

std::vector<std::string> reader(); // We have to define that we have a reader function to read in the run parameters


int templeInversion(int nFreq, const std::string &fileName, const int &rank, const int &nop, const int& nxt, const int& nzt, const int& nSrct, const int& nFreq_Total, const double& Freq_min, const double& Freq_max, const bool& interactive, const double (&reservoir_corner_points_in_m)[2][2], const bool& gpu, const double& c_0);


const int nItReconstructFields = 2; //number of iterations to reconstruct the image

//Here we determine (is_this_our_kind_of_booL) if the reader correctly gave us a Boolean (1 or 0)
//and then set the Boolean from the input string (string_1_for_true_0_for_false)
bool is_this_our_kind_of_bool(std::string const& string_for_bool)
{return (string_for_bool.size() == 1 && (string_for_bool[0] == '0' || string_for_bool[0] == '1'));}
bool string_1_for_true_0_for_false(std::string const& string_for_bool)
{return string_for_bool[0] == '1';}


int main()
{
    // if g_verbosity is 0, print everything to a file program_output.txt, else if g_verbosity = 10, print everything on screen
    if (g_verbosity == 0)
    {
        std::cout << "Printing the program output onto a file named program_output.txt" << std::endl;

        if (freopen("../program_output.txt","w", stdout)) {}
    }

    std::vector<std::string> input_parameters = reader(); // MELISSEN 2018 10 18 we call the reader function to read out the textfile

    //We transfer what is in input_parameters to the relevant constants
    int parameterCounter=0;
    const double    c_0                               = stod(input_parameters[parameterCounter]);    ++parameterCounter; //Speed of sound in background
    const double    c_1                               = stod(input_parameters[parameterCounter]);    ++parameterCounter; //Speed of sound material "1"
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
    const std::string  fileName                       =     (input_parameters[parameterCounter]);    ++parameterCounter; //Filename to be used for inversion
    const double    tol1_to_be_implemented            = stod(input_parameters[parameterCounter]);    ++parameterCounter; //
    const double    tol2_to_be_implemented            = stod(input_parameters[parameterCounter]);    ++parameterCounter; //
    const double    delta_amplification_start_to_be_i = stod(input_parameters[parameterCounter]);    ++parameterCounter; //
    const double    delta_amplification_slope_to_be_i = stod(input_parameters[parameterCounter]);    ++parameterCounter; //
    //  const int       Verbosity                         = stoi(input_parameters[parameterCounter]);    ++parameterCounter; //Saurabh worked on this
    if (!is_this_our_kind_of_bool(input_parameters[parameterCounter])){return 1;}
    const bool      gpu                               = string_1_for_true_0_for_false(input_parameters[parameterCounter]); ++ parameterCounter;
    if (!is_this_our_kind_of_bool(input_parameters[parameterCounter])){return 1;}
    const bool      interactive                       = string_1_for_true_0_for_false(input_parameters[parameterCounter]); ++ parameterCounter;

    const double reservoir_corner_points_in_m[2][2] = {{top_left_corner_coord_x_in_m,top_left_corner_coord_z_in_m},{bottom_right_corner_coord_x_in_m,bottom_right_corner_coord_z_in_m}};

    //MELISSEN 2018 10 18 this snippet of code is meant to be the start of an outputfile that FIRST prints what the run parametrization IS
    //MELISSEN 2018 10 24 PLEASE DO NOT CHANGE THE OUTPUT CODE BELOW ASK ME WHY AT sigismund.melissen@alten.nl
    std::ofstream outputfwi;
    outputfwi.open("../outputfwi.txt");
    outputfwi << "This run was parametrized as follows:"   << std::endl;
    outputfwi << "nxt   = "                                << nxt                                   << std::endl;
    outputfwi << "nzt   = "                                << nzt                                   << std::endl; // etc
    outputfwi << "interactive   = "                        << interactive                           << std::endl; // etc
    outputfwi << "tol1_to_be_implemented    = "            << tol1_to_be_implemented                << std::endl; // etc
    outputfwi << "tol2_to_be_implemented    = "            << tol2_to_be_implemented                << std::endl; // etc
    outputfwi << "delta_amplification_start_to_be_i = "    << delta_amplification_start_to_be_i     << std::endl; // etc
    outputfwi << "delta_amplification_slope_to_be_i = "    << delta_amplification_slope_to_be_i     << std::endl; // etc
    outputfwi << "gpu   = "                                << gpu                                   << std::endl; // etc
    outputfwi << "c_1   = "                                << c_1                                   << std::endl; // etc
    outputfwi.close();

    cout << "Input Temple Visualisation" << endl;
    chi_visualisation_in_integer_form("../temple.txt", nxt);

    // creates a csv file using the reference temple.txt files and saves it as chi_reference_temple.csv file
    create_csv_files_for_chi("../temple.txt","chi_reference_temple",nxt);

    std::time_t start = std::time(nullptr);
    std::cout << "Starting at " <<  std::asctime(std::localtime(&start)) << std::endl;
    int nFreq(nFreq_Total);

    int rank = 0;
    int nop = 1;
    nFreq = nFreq_Total;///nop; //distributing frequencies
    std::cout << "nFreq= " << nFreq << std::endl;
    int ret;
    ret = templeInversion(nFreq, fileName, rank, nop, nxt, nzt, nSrct, nFreq_Total, Freq_min, Freq_max, interactive, reservoir_corner_points_in_m, gpu, c_0);
    std::cout << ret << std::endl;

    cout << "Visualisation of the estimated temple using FWI" << endl;
    chi_visualisation_in_integer_form("../libraries/src/chi_est_temple.txt", nxt);

    // creates a csv file using the final chi_est_temple.txt files and saves it as chi_est_temple.csv file
    create_csv_files_for_chi("../libraries/src/chi_est_temple.txt","chi_est_temple",nxt);

    std::time_t finish = std::time(nullptr);
    std::cout << "Finished at " <<  std::asctime(std::localtime(&finish)) << std::endl;
    return 0;
}


//temple from here

int templeInversion(int nFreq, const std::string &fileName, const int &rank, const int &nop, const int& nxt, const int& nzt, const int& nSrct, const int& nFreq_Total, const double& Freq_min, const double& Freq_max, const bool& interactive, const double (&reservoir_corner_points_in_m)[2][2], const bool& gpu, const double& c_0) // , const int& freq_dist_group)
{
    std::array<double,2> x_min = {(reservoir_corner_points_in_m[0][0]),(reservoir_corner_points_in_m[0][1])}; // Rectangle in meters MELISSEN 2018 10 16 see PhD Haffinger p 53
    std::array<double,2> x_max = {(reservoir_corner_points_in_m[1][0]),(reservoir_corner_points_in_m[1][1])}; // Rectangle in meters MELISSEN see above
    std::array<int,2> ngrid = {nxt, nzt};

    //grid_rect_2D<double> grid(x_min, x_max, ngrid); //Babak 2018 10 29: get rid of template for grid_rect_2D
    grid_rect_2D grid(x_min, x_max, ngrid);
    //volField_rect_2D_cpu<double> chi(grid); //Babak 2018 10 29: get rid of template for grid_rect_2D_cpu
    volField_rect_2D_cpu chi(grid);//Babak 2018 10 29: get rid of template for volField_rect_2D and volField_rect_2D

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
    int freq_left = nFreq_Total - (nFreq_Total/nop)*nop;

    int sum = 0;
    for (int i=0; i<rank; i++)
        sum += nFreq_input[i];
    f_min = Freq_min + sum*d_freq;
    d_freq_proc = d_freq;

    Frequencies_group freq(f_min, d_freq_proc, nFreq, c_0);
    freq.Print();

    ProfileInterface *profiler;
    profiler = new ProfileCpu();

    std::complex<double> *p_data = new std::complex<double>[nFreq * nRecv * nSrct];

    chi.toFile("../libraries/src/chi.txt");

    Inversion *inverse;
    inverse = new InversionConcrete_cpu(grid, src, recv, freq, *profiler);

    std::cout << "Creating Greens function field" << std::endl;
    inverse->createGreens();
    inverse->SetBackground(chi);


    std::cout << "Creating P0..." << std::endl;
    inverse->createP0();


    std::cout << "Creating total field..." << std::endl;
    inverse->createTotalField(rank);


    inverse->calculateData(p_data);


    std::cout << "Estimating Chi..." << std::endl;
    volField_rect_2D_cpu chi_est = inverse->Reconstruct(p_data,rank);

    std::cout << "Done, writing to file" << std::endl;
    chi_est.toFile("../libraries/src/chi_est_temple.txt");

    delete[] p_data;

    return 0;
}

