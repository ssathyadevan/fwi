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


int templeInversion(const std::string &fileName, const int& nxt, const int& nzt, const int& nSrct,
                    const int& nFreq_Total, const double& Freq_min, const double& Freq_max,
                    const double (&reservoir_corner_points_in_m)[2][2], const double& c_0, double tol1_to_be_implemented, double tol2_to_be_implemented,
                    double delta_amplification_start, double delta_amplification_slope, bool calc_alpha, int n_max, int n_iter1, int n_iter2, bool do_reg, std::string runName);


const int nItReconstructFields = 2; //number of iterations to reconstruct the image

//Here we determine (is_this_our_kind_of_bool) if the reader correctly gave us a Boolean (1 or 0)
//and then set the Boolean from the input string (string_1_for_true_0_for_false)
bool is_this_our_kind_of_bool(std::string const& string_for_bool)
{return (string_for_bool.size() == 1 && (string_for_bool[0] == '0' || string_for_bool[0] == '1'));}
bool string_1_for_true_0_for_false(std::string const& string_for_bool)
{return string_for_bool[0] == '1';}


int main(int argc, char** argv)
{
    std::string runName;
    if (argc != 2)
	{
    runName = "default";
	} 
   else
	{
	runName = argv[1];
	}
    // if g_verbosity is 0, print everything to a file program_output.txt, else if g_verbosity = 10, print everything on screen
    if (g_verbosity == 0)
    {
        std::cout << "Printing the program output onto a file named: "+runName+".out in the inputOutput folder" << std::endl;

        std::string tempString = "../inputOutput/" + runName + ".out";
        if (freopen(tempString.c_str(),"w", stdout)) {}
    }

    std::vector<std::string> input_parameters = reader(runName); // MELISSEN 2018 10 18 we call the reader function to read out the textfile

    //We transfer what is in input_parameters to the relevant constants
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


    const double reservoir_corner_points_in_m[2][2] = {{top_left_corner_coord_x_in_m,top_left_corner_coord_z_in_m},{bottom_right_corner_coord_x_in_m,bottom_right_corner_coord_z_in_m}};


    std::ofstream outputfwi;
    outputfwi.open("../inputOutput/" + runName + ".pythonIn");
    outputfwi << "This run was parametrized as follows:"   << std::endl;
    outputfwi << "nxt   = "                                << nxt                                   << std::endl;
    outputfwi << "nzt   = "                                << nzt                                   << std::endl; // etc
    outputfwi.close();

    std::ofstream lastrun;
    lastrun.open("../inputOutput/lastRunName.txt");
    lastrun << runName;
    lastrun.close();


    cout << "Input Temple Visualisation" << endl;

    chi_visualisation_in_integer_form("../"+fileName+".txt", nxt);

    // creates a csv file using the reference temple.txt files and saves it as chi_reference_temple.csv file

    create_csv_files_for_chi("../"+fileName+".txt","chi_reference_"+runName,nxt);


    std::time_t start = std::time(nullptr);
    std::cout << "Starting at " <<  std::asctime(std::localtime(&start)) << std::endl;
    //int nFreq(nFreq_Total);


    //nFreq = nFreq_Total;
    std::cout << "nFreq_Total= " << nFreq_Total << std::endl;
    int ret;
    ret = templeInversion(fileName, nxt, nzt, nSrct, nFreq_Total, Freq_min, Freq_max,
                          reservoir_corner_points_in_m, c_0, tol1_to_be_implemented, tol2_to_be_implemented, delta_amplification_start,
                          delta_amplification_slope, calc_alpha, n_max, n_iter1, n_iter2, do_reg, runName);
    std::cout << ret << std::endl;

    cout << "Visualisation of the estimated temple using FWI" << endl;
    chi_visualisation_in_integer_form("../inputOutput/chi_est_"+runName+".txt", nxt);

    // creates a csv file using the final chi_est_temple.txt files and saves it as chi_est_temple.csv file
    create_csv_files_for_chi("../inputOutput/chi_est_"+runName+".txt","chi_est_"+runName,nxt);

    std::time_t finish = std::time(nullptr);
    std::cout << "Finished at " <<  std::asctime(std::localtime(&finish)) << std::endl;
    return 0;
}


//temple from here

int templeInversion (const std::string &fileName, const int& nxt, const int& nzt, const int& nSrct,
 const int& nFreq_Total, const double& Freq_min, const double& Freq_max,
 const double (&reservoir_corner_points_in_m)[2][2], const double& c_0, double tol1_to_be_implemented, double tol2_to_be_implemented,
 double delta_amplification_start, double delta_amplification_slope, bool calc_alpha, int n_max, int n_iter1, int n_iter2, bool do_reg, std::string runName) // , const int& freq_dist_group)
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

    int sum = 0;


    f_min = Freq_min + sum*d_freq;
    d_freq_proc = d_freq;

    Frequencies_group freq(f_min, d_freq_proc, nFreq_Total, c_0);
    freq.Print();

    ProfileInterface *profiler;
    profiler = new ProfileCpu();

    std::complex<double> *p_data = new std::complex<double>[nFreq_Total * nRecv * nSrct];

    chi.toFile("../inputOutput/chi_ref_"+runName+".txt");

    Inversion *inverse;
    inverse = new InversionConcrete_cpu(grid, src, recv, freq, *profiler);

    std::cout << "Creating Greens function field" << std::endl;
    inverse->createGreens();
    inverse->SetBackground(chi);


    std::cout << "Creating P0..." << std::endl;
    inverse->createP0();


    std::cout << "Creating total field..." << std::endl;
    inverse->createTotalField(tol2_to_be_implemented, calc_alpha, n_iter2);


    inverse->calculateData(p_data);


    std::cout << "Estimating Chi..." << std::endl;
    volField_rect_2D_cpu chi_est = inverse->Reconstruct(p_data, tol1_to_be_implemented, tol2_to_be_implemented, delta_amplification_start, delta_amplification_slope,
                                                        calc_alpha, n_max, n_iter1, n_iter2, do_reg);

    std::cout << "Done, writing to file" << std::endl;
    chi_est.toFile("../inputOutput/chi_est_"+runName+".txt");

    delete[] p_data;

    return 0;
}

