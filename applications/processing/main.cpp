

#include "inversionRandom.h"
#include "inversion.h"

#include "inputCardReader.h"
#include "utilityFunctions.h"
#include "chiIntegerVisualisation.h"
#include "createChiCSV.h"
#include "csvReader.h"
#include "cpuClock.h"


void performInversion(const Input& input);

int main(int argc, char** argv)
{
    if (argc != 4)
    {
        std::cout<< "Please enter 3 arguments, 1st the input card path, 2nd the output folder location and 3rd the input card name" << std::endl;
        std::cout<< "e.g. ./FWI_Process ~/Documents/FWIInstall/ ~/Documents/FWIInstall/Output/ cardName" << std::endl;

        exit(EXIT_FAILURE);
    }
    std::vector<std::string> arguments(argv+1, argc+argv);

    Input input = inputCardReader(arguments[0], arguments[1], arguments[2]);

    if (!input.verbose)
    {
        WriteToFileNotToTerminal(input.outputLocation, input.cardName, "Process");
    }

    chi_visualisation_in_integer_form(input.inputCardPath + input.fileName + ".txt", input.ngrid[0]);
    create_csv_files_for_chi(input.inputCardPath + input.fileName + ".txt", input, "chi_reference_");

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
    grid2D grid(input.reservoirTopLeftCornerInM, input.reservoirBottomRightCornerInM, input.ngrid);
    sources src(input.sourcesTopLeftCornerInM, input.sourcesBottomRightCornerInM, input.nSourcesReceivers.src);
    src.Print();
    receivers recv(src);
    recv.Print();
    frequenciesGroup freqg(input.freq, input.c_0);
    freqg.Print(input.freq.nTotal);

    int magnitude = input.freq.nTotal * input.nSourcesReceivers.src * input.nSourcesReceivers.rec;
    //read referencePressureData from a CSV file format
    std::complex<double> referencePressureData[magnitude];
    std::ifstream       file(input.outputLocation+input.cardName+"InvertedChiToPressure.txt");
    CSVReader           row;
    int i = 0;
    while(file >> row)
    {
        if (i<magnitude)
        {
            referencePressureData[i]= { atof(row[0].c_str() ), atof(row[1].c_str()) };
        }
        i++;
    }

    ForwardModelInterface *model;
    model = new forwardModel(grid, src, recv, freqg, input);


    inversionInterface *inverse;
//    inverse = new inversionRandom(model);

//    inversionInterface *inverse;
      inverse = new inversion(model);


    std::cout << "Estimating Chi..." << std::endl;

    pressureFieldSerial chi_est = inverse->Reconstruct(referencePressureData, input);

    std::cout << "Done, writing to file" << std::endl;

    chi_est.toFile(input.outputLocation + "chi_est_"+ input.cardName+ ".txt");

    delete model;
    delete inverse;

}
