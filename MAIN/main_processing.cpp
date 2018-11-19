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
#include "InversionConcrete_cpu.h"
#include "read_input_fwi_into_vec.h"
#include "variable_structure.h"
#include "communication.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ios>
#include <vector>
#include "chi_visualisation_in_integer_form.h"
#include "create_csv_files_for_chi.h"
#include <stdexcept>
#include "CsvReader.h"

// Babak and Saurabh 2018-11-13: Creating the pre-processing programm.

// Walkthrough ("WT") of this file is made by S. Melissen, 2018-11-16
// This "WT" is just 4 sequential comments on what happens
int performInversion
    (const std::string, const NSourcesReceivers, const Freq, const double,
     const ConjGrad, const std::string, std::array<int,2>,
     std::array<double,2>, std::array<double,2>,
     std::array<double,2>, std::array<double,2>,
     const Input);

int main(int argc, char** argv)
{
    // WT 1/4: See "variable_structure.h" to see what the struct "Input" is...
    // ...and "read_input....h" to see how we turn input card into parameters...
    // ...argv is just another parameter, the filename containing the others.
    Input input = reader3(argc, argv);

    if (!input.verbose)
	WriteToFileNotToTerminal(input.runName);

    ClockStart(input.freq.nTotal); // WT 2/4: Start clock & cout info

    chi_visualisation_in_integer_form("../"+input.fileName+".txt", input.ngrid[0]);

    // creates a csv file using the reference temple.txt files and saves it as chi_reference_temple.csv file

    create_csv_files_for_chi("../"+input.fileName+".txt","chi_ref_"+input.runName,input.ngrid[0]);

    int ret = performInversion
	(input.fileName, input.nSourcesReceivers, input.freq, input.c_0,
         input.conjGrad, input.runName, input.ngrid,
	 input.reservoirTopLeftCornerInM, input.reservoirBottomRightCornerInM,
         input.sourcesTopLeftCornerInM, input.sourcesBottomRightCornerInM,
	 input);

    cout << "Visualisation of the estimated temple using FWI" << endl;
    chi_visualisation_in_integer_form("../inputOutput/chi_est_"+input.runName+".txt", input.ngrid[0]);

    // creates a csv file using the final chi_est_temple.txt files and saves it as chi_est_temple.csv file
    create_csv_files_for_chi("../inputOutput/chi_est_"+input.runName+".txt","chi_est_"+input.runName,input.ngrid[0]);

    ClockStop(ret); // WT 3/4: Stop clock & cout whether successful

    return 0;
}

// WT 4/4: Here the mathematics of the "preprocessing" part of FWI is done.
int performInversion
    (const std::string fileName, const NSourcesReceivers nSourcesReceivers,
     const Freq freq, const double c_0,
     const ConjGrad conjGrad, const std::string runName, std::array<int,2> ngrid,
     std::array<double,2> reservoirTopLeftCornerInM,
     std::array<double,2> reservoirBottomRightCornerInM,
     std::array<double,2> sourcesTopLeftCornerInM,
     std::array<double,2> sourcesBottomRightCornerInM,
     Input input) {

    grid_rect_2D grid(reservoirTopLeftCornerInM, reservoirBottomRightCornerInM, ngrid);
    volField_rect_2D_cpu chi(grid);
    chi.fromFile(fileName);

    Sources_rect_2D src(sourcesTopLeftCornerInM, sourcesBottomRightCornerInM, nSourcesReceivers.src);
    src.Print();

    Receivers_rect_2D recv(src);
    recv.Print();

    Frequencies_group freqg(freq, c_0);
    freqg.Print(freq.nTotal);

    ProfileInterface *profiler;
    profiler = new ProfileCpu();

    int magnitude = freq.nTotal * nSourcesReceivers.src * nSourcesReceivers.rec;

    //read referencePressureData
    std::complex<double> referencePressureData[magnitude];
    std::ifstream       file("../inputOutput/"+runName+"InvertedChiToPressure.txt");
    CSVRow              row;
    int i = 0;
    while(file >> row)
    {
        if (i<magnitude) { referencePressureData[i]= { atof(row[0].c_str()), atof(row[1].c_str())};}
        i++;
    }

    Inversion *inverse;
    inverse = new InversionConcrete_cpu(grid, src, recv, freqg, *profiler, chi);

    std::cout << "Estimating Chi..." << std::endl;
    volField_rect_2D_cpu chi_est = inverse->Reconstruct(referencePressureData, input.iter1, conjGrad, input.deltaAmplification, input.n_max, input.do_reg);
    std::cout << "Done, writing to file" << std::endl;

    chi_est.toFile("../inputOutput/chi_est_"+runName+".txt");

    return 0;
}
