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
#include "inversionInterface.h"
#include "Inversion.h"
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


int performInversion(const Input& input);

int main(int argc, char** argv)
{
    Input input = reader3(argc, argv);

    if (!input.verbose)
    {
        WriteToFileNotToTerminal(input.runName, "Process");
    }

    ClockStart(input.freq.nTotal);

    chi_visualisation_in_integer_form("../" + input.fileName + ".txt", input.ngrid[0]);

    create_csv_files_for_chi("../../../parallelized-fwi/"+input.fileName+".txt","chi_ref_"+input.runName,input.ngrid[0]);

    int ret = performInversion(input);

    cout << "Visualisation of the estimated temple using FWI" << endl;

    chi_visualisation_in_integer_form("../../../parallelized-fwi/inputOutput/chi_est_"+input.runName+".txt", input.ngrid[0]);

    create_csv_files_for_chi("../../../parallelized-fwi/inputOutput/chi_est_"+input.runName+".txt","chi_est_"+input.runName,input.ngrid[0]);

    ClockStop(ret);

    return 0;
}

int performInversion(const Input& input)
{
    #include "setupInputParametersForFurtherCalculations.h"

    int magnitude = input.freq.nTotal * input.nSourcesReceivers.src * input.nSourcesReceivers.rec;

    //read referencePressureData from a CSV file format
    std::complex<double> referencePressureData[magnitude];
    std::ifstream       file("../../../parallelized-fwi/inputOutput/"+input.runName+"InvertedChiToPressure.txt");
    CSVRow              row;
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
    forwardModel = new ForwardModel(grid, src, recv, freqg, *profiler, input);

    InversionInterface *inverse;
    inverse = new Inversion(forwardModel);

    std::cout << "Estimating Chi..." << std::endl;

    volField_rect_2D_cpu chi_est = inverse->Reconstruct(referencePressureData, input);

    std::cout << "Done, writing to file" << std::endl;

    chi_est.toFile("../../../parallelized-fwi/inputOutput/chi_est_"+input.runName+".txt");

    delete forwardModel;

    return 0;
}
