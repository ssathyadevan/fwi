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


// Walkthrough ("WT") of this file, 2018-11-20
// This "WT" is just 4 sequential comments on what happens
int performInversion(const Input& input);

int main(int argc, char** argv)
{
    // WT 1/5: See "variable_structure.h" to see what the struct "Input" is...
    // ...and "read_input....h" to see how we turn input card into parameters...
    // ...argv is just another parameter i.e. the "filename" of the input card.
    Input input = reader3(argc, argv);

    if (!input.verbose) { WriteToFileNotToTerminal(input.runName, "Process");}

    ClockStart(input.freq.nTotal); // WT 2/5: Start clock & cout info

    chi_visualisation_in_integer_form("../"+input.fileName+".txt", input.ngrid[0]);

    create_csv_files_for_chi("../../../"+input.fileName+".txt","chi_ref_"+input.runName,input.ngrid[0]);

    // WT3/5: The full waveform inversion method is called by providing the ...
    //... relevant input arguments from the "input" struct created above
    int ret = performInversion(input);

    cout << "Visualisation of the estimated temple using FWI" << endl;
    chi_visualisation_in_integer_form("../../../inputOutput/chi_est_"+input.runName+".txt", input.ngrid[0]);

    create_csv_files_for_chi("../../../inputOutput/chi_est_"+input.runName+".txt","chi_est_"+input.runName,input.ngrid[0]);

    ClockStop(ret); // WT 4/5: Stop clock & cout whether successful

    return 0;
}

// WT 5/5: Here the mathematics of the "Inversion" part of FWI is done.
int performInversion(const Input& input) {

    #include "setupInputParametersForFurtherCalculations.h"

    int magnitude = input.freq.nTotal * input.nSourcesReceivers.src * input.nSourcesReceivers.rec;

    //read referencePressureData from a CSV file format
    std::complex<double> referencePressureData[magnitude];
    std::ifstream       file("../../../inputOutput/"+input.runName+"InvertedChiToPressure.txt");
    CSVRow              row;
    int i = 0;
    while(file >> row) {
        if (i<magnitude) { referencePressureData[i]= { atof(row[0].c_str()), atof(row[1].c_str())}; }
        i++;
    }

    ForwardModelInterface *forwardModel;
    forwardModel = new ForwardModel(grid, src, recv, freqg, *profiler, chi);

    Inversion *inverse;
    //inverse = new InversionConcrete_cpu(forwardModel, grid, src, recv, freqg, *profiler, chi);
    inverse = new InversionConcrete_cpu(forwardModel);


    std::cout << "Estimating Chi..." << std::endl;
    volField_rect_2D_cpu chi_est = inverse->Reconstruct(referencePressureData, input.iter1, input.conjGrad, input.deltaAmplification, input.n_max, input.do_reg);
    std::cout << "Done, writing to file" << std::endl;

    chi_est.toFile("../../../inputOutput/chi_est_"+input.runName+".txt");

    return 0;
}
