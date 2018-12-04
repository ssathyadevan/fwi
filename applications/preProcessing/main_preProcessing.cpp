#include "read_input_fwi_into_vec.h"
#include "communication.h"
//#include "chi_visualisation_in_integer_form.h"
//#include "create_csv_files_for_chi.h"
#include "sources_rect_2D.h"
#include "receivers_rect_2D.h"
#include "forwardModel.h"



// Walkthrough ("WT") of this file is made by S. Melissen, 2018-11-16
// This "WT" is just 4 sequential comments on what happens
int generateReferencePressureFieldFromChi(const Input& input);

int main(int argc, char** argv)
{
    // WT 1/4: See "variable_structure.h" to see what the struct "Input" is...
    // ...and "read_input....h" to see how we turn input card into parameters...
    // ...argv is just another parameter i.e. the "filename" of the input card.
    Input input = reader3(argc, argv);

    if (!input.verbose)
        WriteToFileNotToTerminal(input.runName, "PreProcess");

    ClockStart(input.freq.nTotal); // WT 2/4: Start clock & cout info

    int ret =generateReferencePressureFieldFromChi(input);

    ClockStop(ret); // WT 3/4: Stop clock & cout whether successful

    return 0;
}

// WT 4/4: Here the mathematics of the "preprocessing" part of FWI is done.
int generateReferencePressureFieldFromChi
(const Input& input) {

    #include "setupInputParametersForFurtherCalculations.h"

    int magnitude = input.freq.nTotal * input.nSourcesReceivers.src * input.nSourcesReceivers.rec;

    std::complex<double> referencePressureData[magnitude];

    chi.toFile("../../../inputOutput/chi_ref_" + input.runName + ".txt");

    ForwardModelInterface *forwardModel;
    forwardModel = new ForwardModel(grid, src, recv, freqg, *profiler);

    std::cout << "Creating total field..." << std::endl;
    forwardModel->createTotalField(input.conjGrad, chi);

    std::cout << "Calculate pData (the reference pressure-field)..." << std::endl;
    forwardModel->calculateData(referencePressureData, chi);

    // writing the referencePressureData to a text file in complex form
    std::string invertedChiToPressureFileName = "../../../inputOutput/"+input.runName+"InvertedChiToPressure.txt";
    std::ofstream file;
    file.open (invertedChiToPressureFileName, std::ios::out | std::ios::trunc);
    assert(file.is_open());
    for(int i=0; i < magnitude; i++) {
        file << std::setprecision(17) << referencePressureData[i].real()
             <<"," << referencePressureData[i].imag() << std::endl;
    }
    file.close();
    
    return 0;
}


