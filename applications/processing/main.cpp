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
#include "sources_rect_2D.h"
#include "receivers_rect_2D.h"
#include "frequencies_group.h"
#include "inversionInterface.h"
#include "Inversion.h"
#include "read_input_fwi_into_vec.h"
#include "variable_structure.h"
#include "utilityFunctions.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ios>
#include <vector>
#include "chi_visualisation_in_integer_form.h"
#include "create_csv_files_for_chi.h"
#include <stdexcept>
#include "CsvReader.h"
#include "cpuClock.h"


void performInversion(const Input& input);

int main(int argc, char** argv)
{
    Input input = reader3(argc, argv);

    if (!input.verbose)
    {
        WriteToFileNotToTerminal(input.outputLocation, input.cardName, "Process");
    }

    chi_visualisation_in_integer_form(input.inputCardPath + input.fileName + ".txt", input.ngrid[0]);
    create_csv_files_for_chi(input.inputCardPath + input.fileName + ".txt", input, "chi_ref_");

    cpuClock clock;

    clock.Start();
    performInversion(input);
    clock.End();
    clock.PrintTimeElapsed();

    cout << "Visualisation of the estimated temple using FWI" << endl;
    chi_visualisation_in_integer_form(input.outputLocation + "chi_est_" + input.cardName + ".txt", input.ngrid[0]);
    create_csv_files_for_chi(input.outputLocation + "chi_est_" + input.cardName + ".txt", input, "chi_est_");

    return 0;
}

void performInversion(const Input& input)
{
    // initialize the grid, sources, receivers, grouped frequencies
    grid_rect_2D grid(input.reservoirTopLeftCornerInM, input.reservoirBottomRightCornerInM, input.ngrid);
    Sources_rect_2D src(input.sourcesTopLeftCornerInM, input.sourcesBottomRightCornerInM, input.nSourcesReceivers.src);
    src.Print();
    Receivers_rect_2D recv(src);
    recv.Print();
    Frequencies_group freqg(input.freq, input.c_0);
    freqg.Print(input.freq.nTotal);

    int magnitude = input.freq.nTotal * input.nSourcesReceivers.src * input.nSourcesReceivers.rec;
    //read referencePressureData from a CSV file format
    std::complex<double> referencePressureData[magnitude];
    std::ifstream       file(input.outputLocation+input.cardName+"InvertedChiToPressure.txt");
    CSVReader              row;
    int i = 0;
    while(file >> row)
    {
        if (i<magnitude)
        {
            referencePressureData[i]= { atof(row[0].c_str() ), atof(row[1].c_str()) };
        }
        i++;
    }

    ForwardModelInterface *forwardModel;
    forwardModel = new ForwardModel(grid, src, recv, freqg, input);

    InversionInterface *inverse;
    inverse = new Inversion(forwardModel);

    std::cout << "Estimating Chi..." << std::endl;

    volField_rect_2D_cpu chi_est = inverse->Reconstruct(referencePressureData, input);

    std::cout << "Done, writing to file" << std::endl;

    //chi_est.toFile("../../../parallelized-fwi/inputOutput/chi_est_"+input.outputLocation+".txt");

    chi_est.toFile(input.outputLocation + "chi_est_"+ input.cardName+ ".txt");

    delete forwardModel;
    delete inverse;

}
