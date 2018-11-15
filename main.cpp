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

using std::cout; using std::endl; using std::string;

/*
Babak and Saurabh 2018-11-13: Creating the pre-processing programm.
*/

int generateReferencePressureFieldFromChi
    (const std::string&, const int&, const Freq&, const double (&)[2][2],
     const double&, const ConjGrad&, std::string, std::array<int,2>);

int main(int argc, char** argv)
{
    std::array<string, 2> inputPathNName = InputPathNName(argc, argv);
    string findPath=inputPathNName[0]; string runName=inputPathNName[1];

    Input input = reader2(findPath + runName + ".in");

    if (!input.verbose)
    {
        std::cout << "Printing the program output onto a file named: "+runName+".out in the inputOutput folder" << std::endl;
        std::string tempString = "../inputOutput/" + runName + ".out";
        if (freopen(tempString.c_str(),"w", stdout)) {}
    }

    // This part is needed for plotting the chi values in imageCreator_CMake.py
    std::ofstream outputfwi;
    outputfwi.open("../inputOutput/" + runName + ".pythonIn");
    outputfwi << "This run was parametrized as follows:" << std::endl;
    outputfwi << "nxt   = " << input.ngrid[0] << std::endl;
    outputfwi << "nzt   = " << input.ngrid[1] << std::endl;
    outputfwi.close();
    // This part is needed for plotting the chi values in imageCreator_CMake.py
    std::ofstream lastrun;
    lastrun.open("../inputOutput/lastRunName.txt");
    lastrun << runName;
    lastrun.close();

    cout << "Preprocessing the provided input to create the reference pressure-field" << endl;
    std::time_t start = std::time(nullptr);
    std::cout << "Starting at " <<  std::asctime(std::localtime(&start)) << std::endl;
    std::cout << "input.freq.nTotal= " << input.freq.nTotal << std::endl;
    // Generating the reference pressure field from input data (chi values)
    int ret =generateReferencePressureFieldFromChi(input.fileName,// nxt, nzt
                                                  input.nSourcesReceivers.src,
                                                  input.freq,
						  //input.freq.nTotal, input.freq.min, input.freq.max,
                                                  input.reservoirCornerPointsInM, input.c_0,
                                                  input.conjGrad,//.tolerance, input.conjGrad.calcAlpha, input.conjGrad.nIter,
                                                  runName, input.ngrid);
    std::cout << ret << std::endl;
    std::time_t finish = std::time(nullptr);
    std::cout << "Finished at " <<  std::asctime(std::localtime(&finish)) << std::endl;
    return 0;
}

int generateReferencePressureFieldFromChi (const std::string &fileName,//const int& nxt, const int& nzt, 
		                          const int& nSrct,
                                          const Freq& freq,
					  //const int& nFreq_Total, const double& Freq_min, const double& Freq_max,
                                          const double (&reservoir_corner_points_in_m)[2][2], const double& c_0,
                                          const ConjGrad& conjGrad,
                                          //double tol2_to_be_implemented, bool calc_alpha, int n_iter2,
                                          std::string runName, std::array<int,2> ngrid2)
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

    double d_freq = (freq.max - freq.min)/(freq.nTotal  -1);

    int sum = 0;


    f_min = freq.min + sum*d_freq;
    d_freq_proc = d_freq;

    Frequencies_group freqg(f_min, d_freq_proc, freq.nTotal, c_0);
    freqg.Print();

    ProfileInterface *profiler;
    profiler = new ProfileCpu();

    std::complex<double> referencePressureData[freq.nTotal * nRecv * nSrct];

    chi.toFile("../inputOutput/chi_ref_" + runName + ".txt");

    Inversion *inverse;
    inverse = new InversionConcrete_cpu(grid, src, recv, freqg, *profiler);

    std::cout << "Creating Greens function field..." << std::endl;
    inverse->createGreens();
    inverse->SetBackground(chi);

    std::cout << "Creating P0..." << std::endl;
    inverse->createP0();

    std::cout << "Creating total field..." << std::endl;
    inverse->createTotalField(conjGrad.tolerance, conjGrad.calcAlpha, conjGrad.nIter);

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
    for(int i=0; i < freq.nTotal * nRecv * nSrct; i++) {
        file << std::setprecision(17) << referencePressureData[i].real() <<"," << referencePressureData[i].imag() << std::endl;
    }
    file.close();
    return 0;
}
