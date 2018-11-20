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

// Babak and Saurabh 2018-11-13: Creating the pre-processing program.

// Walkthrough ("WT") of this file is made by S. Melissen, 2018-11-16
// This "WT" is just 4 sequential comments on what happens
int generateReferencePressureFieldFromChi
(const std::string, const NSourcesReceivers, const Freq, const double,
 const ConjGrad, const std::string, std::array<int,2>,
 std::array<double,2>, std::array<double,2>,
 std::array<double,2>, std::array<double,2>);

int main(int argc, char** argv)
{
    // WT 1/4: See "variable_structure.h" to see what the struct "Input" is...
    // ...and "read_input....h" to see how we turn input card into parameters...
    // ...argv is just another parameter i.e. the "filename" of the input card.
    Input input = reader3(argc, argv);

    if (!input.verbose)
        WriteToFileNotToTerminal(input.runName, "PreProcess");

    ClockStart(input.freq.nTotal); // WT 2/4: Start clock & cout info

    int ret =generateReferencePressureFieldFromChi
            (input.fileName, input.nSourcesReceivers, input.freq, input.c_0,
             input.conjGrad, input.runName, input.ngrid,
             input.reservoirTopLeftCornerInM, input.reservoirBottomRightCornerInM,
             input.sourcesTopLeftCornerInM, input.sourcesBottomRightCornerInM);

    ClockStop(ret); // WT 3/4: Stop clock & cout whether successful

    return 0;
}

// WT 4/4: Here the mathematics of the "preprocessing" part of FWI is done.
int generateReferencePressureFieldFromChi
(const std::string fileName, const NSourcesReceivers nSourcesReceivers,
 const Freq freq, const double c_0,
 const ConjGrad conjGrad, const std::string runName, std::array<int,2> ngrid,
 std::array<double,2> reservoirTopLeftCornerInM,
 std::array<double,2> reservoirBottomRightCornerInM,
 std::array<double,2> sourcesTopLeftCornerInM,
 std::array<double,2> sourcesBottomRightCornerInM) {

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

    std::complex<double> referencePressureData[magnitude];

    chi.toFile("../../../inputOutput/chi_ref_" + runName + ".txt");

    Inversion *inverse;
    inverse = new InversionConcrete_cpu(grid, src, recv, freqg, *profiler, chi);

    std::cout << "Creating total field..." << std::endl;
    inverse->createTotalField(conjGrad);

    std::cout << "Calculate pData (the reference pressure-field)..." << std::endl;
    inverse->calculateData(referencePressureData);
    // writing the referencePressureData to a text file in complex form
    std::string invertedChiToPressureFileName = "../../../inputOutput/"+runName+"InvertedChiToPressure.txt";

    std::ofstream file;
    file.open (invertedChiToPressureFileName, std::ios::out | std::ios::trunc);
    assert(file.is_open());
    /*    file << std::setprecision(17) << "# This is the file which contains the reference pressure"
                                     " data obtained from the reference chi values in the form "
                                     "of complex numbers." << std::endl;
    file << std::setprecision(17) << "# First column indicates the real values and the second "
                                     "column indicates the imaginary values." << std::endl;*/
    for(int i=0; i < magnitude; i++) {
        file << std::setprecision(17) << referencePressureData[i].real()
             <<"," << referencePressureData[i].imag() << std::endl;
    }

    file.close();
    
    return 0;
}


