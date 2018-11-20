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

// Saurabh 2018-11-19: Creating the processing program.

// Walkthrough ("WT") of this file, 2018-11-20
// This "WT" is just 4 sequential comments on what happens
int performInversion
(const std::string, const NSourcesReceivers, const Freq, const double,
 const ConjGrad, const std::string, std::array<int,2>,
 std::array<double,2>, std::array<double,2>,
 std::array<double,2>, std::array<double,2>,
 DeltaAmplification deltaAmplification,
 Iter1 iter1,
 int n_max,
 bool do_reg);

int main(int argc, char** argv)
{
    // WT 1/5: See "variable_structure.h" to see what the struct "Input" is...
    // ...and "read_input....h" to see how we turn input card into parameters...
    // ...argv is just another parameter i.e. the "filename" of the input card.
    Input input = reader3(argc, argv);

    if (!input.verbose)
        WriteToFileNotToTerminal(input.runName, "Process");

    ClockStart(input.freq.nTotal); // WT 2/5: Start clock & cout info

    chi_visualisation_in_integer_form("../"+input.fileName+".txt", input.ngrid[0]);

    create_csv_files_for_chi("../../../"+input.fileName+".txt","chi_ref_"+input.runName,input.ngrid[0]);

    // WT3/5: The full waveform inversion method is called by providing the ...
    //... relevant input arguments from the "input" struct created above
    int ret = performInversion
            (input.fileName, input.nSourcesReceivers, input.freq, input.c_0,
             input.conjGrad, input.runName, input.ngrid,
             input.reservoirTopLeftCornerInM, input.reservoirBottomRightCornerInM,
             input.sourcesTopLeftCornerInM, input.sourcesBottomRightCornerInM,
             input.deltaAmplification, input.iter1, input.n_max, input.do_reg);

    cout << "Visualisation of the estimated temple using FWI" << endl;
    chi_visualisation_in_integer_form("../../../inputOutput/chi_est_"+input.runName+".txt", input.ngrid[0]);

    create_csv_files_for_chi("../../../inputOutput/chi_est_"+input.runName+".txt","chi_est_"+input.runName,input.ngrid[0]);

    ClockStop(ret); // WT 4/5: Stop clock & cout whether successful

    return 0;
}

// WT 5/5: Here the mathematics of the "Inversion" part of FWI is done.
int performInversion
(const std::string fileName, const NSourcesReceivers nSourcesReceivers,
 const Freq freq, const double c_0,
 const ConjGrad conjGrad, const std::string runName, std::array<int,2> ngrid,
 std::array<double,2> reservoirTopLeftCornerInM,
 std::array<double,2> reservoirBottomRightCornerInM,
 std::array<double,2> sourcesTopLeftCornerInM,
 std::array<double,2> sourcesBottomRightCornerInM,
 DeltaAmplification deltaAmplification,
 Iter1 iter1,
 int n_max,
 bool do_reg) {

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

    //read referencePressureData from a CSV file format
    std::complex<double> referencePressureData[magnitude];
    std::ifstream       file("../../../inputOutput/"+runName+"InvertedChiToPressure.txt");
    CSVRow              row;
    int i = 0;
    while(file >> row)
    {
        if (i<magnitude) { referencePressureData[i]= { atof(row[0].c_str()), atof(row[1].c_str())}; }
        i++;
    }

    Inversion *inverse;
    inverse = new InversionConcrete_cpu(grid, src, recv, freqg, *profiler, chi);

    std::cout << "Estimating Chi..." << std::endl;
    volField_rect_2D_cpu chi_est = inverse->Reconstruct(referencePressureData, iter1, conjGrad, deltaAmplification, n_max, do_reg);
    std::cout << "Done, writing to file" << std::endl;

    chi_est.toFile("../../../inputOutput/chi_est_"+runName+".txt");

    return 0;
}
