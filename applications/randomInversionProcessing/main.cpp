
#include "genericInputCardReader.h"
#include "randomInversionInputCardReader.h"
#include "forwardModelInputCardReader.h"
#include "inversionRandom.h"
#include "inputCardReader.h"
#include "utilityFunctions.h"
#include "chiIntegerVisualisation.h"
#include "createChiCSV.h"
#include "csvReader.h"
#include "cpuClock.h"


void performInversion(const genericInput &gInput, const forwardModelInput &fmInput, const randomInversionInput &riInput);

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::cout << "Please enter 2 arguments, 1st the input card path, 2nd the output folder location," << std::endl;
        std::cout << "Make sure the input folder contains the GenericInput.in, ForwardModelInput.in and RandomInversionInput" << std::endl;
        std::cout << "e.g. ~/Documents/FWIInstall/Input/ ~/Documents/FWIInstall/Output/" << std::endl;

        exit(EXIT_FAILURE);
    }
    std::vector<std::string> arguments(argv+1, argc+argv);

    genericInputCardReader genericReader = genericInputCardReader(arguments[0], arguments[1], "GenericInput");
    forwardModelInputCardReader forwardModelReader = forwardModelInputCardReader(arguments[0], arguments[1], "ForwardModelInput");
    randomInversionInputCardReader randomInversionReader = randomInversionInputCardReader(arguments[0], arguments[1], "RandomInversionInput");
    genericInput gInput = genericReader.getInput();
    forwardModelInput fmInput = forwardModelReader.getInput();
    randomInversionInput riInput = randomInversionReader.getInput();



    if (!gInput.verbose)
    {
        WriteToFileNotToTerminal(gInput.outputLocation, gInput.cardName, "Process");
    }

    chi_visualisation_in_integer_form(gInput.inputCardPath + gInput.fileName + ".txt", gInput.ngrid[0]);
    create_csv_files_for_chi(gInput.inputCardPath + gInput.fileName + ".txt", gInput, "chi_reference_");

    cpuClock clock;

    clock.Start();
    performInversion(gInput, fmInput, riInput);
    clock.End();
    clock.PrintTimeElapsed();

    cout << "Visualisation of the estimated temple using FWI" << endl;
    chi_visualisation_in_integer_form(gInput.outputLocation + "chi_est_" + gInput.cardName + ".txt", gInput.ngrid[0]);
    create_csv_files_for_chi(gInput.outputLocation + "chi_est_" + gInput.cardName + ".txt", gInput, "chi_est_");

    return 0;
}

void performInversion(const genericInput &gInput, const forwardModelInput &fmInput, const randomInversionInput &riInput)
{
    // initialize the grid, sources, receivers, grouped frequencies
    grid2D grid(gInput.reservoirTopLeftCornerInM, gInput.reservoirBottomRightCornerInM, gInput.ngrid);
    sources src(gInput.sourcesTopLeftCornerInM, gInput.sourcesBottomRightCornerInM, gInput.nSourcesReceivers.src);
    src.Print();
    receivers recv(src);
    recv.Print();
    frequenciesGroup freqg(gInput.freq, gInput.c_0);
    freqg.Print(gInput.freq.nTotal);

    int magnitude = gInput.freq.nTotal * gInput.nSourcesReceivers.src * gInput.nSourcesReceivers.rec;
    //read referencePressureData from a CSV file format
    std::complex<double> referencePressureData[magnitude];
    std::ifstream       file(gInput.outputLocation+gInput.cardName+"InvertedChiToPressure.txt");
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
    model = new forwardModel(grid, src, recv, freqg, gInput, fmInput);


    inversionInterface *inverse;
    inverse = new inversionRandom(model, riInput);

//    inversionInterface *inverse;
//    inverse = new inversion(model);


    std::cout << "Estimating Chi..." << std::endl;

    pressureFieldSerial chi_est = inverse->Reconstruct(referencePressureData, gInput);

    std::cout << "Done, writing to file" << std::endl;

    chi_est.toFile(gInput.outputLocation + "chi_est_"+ gInput.cardName+ ".txt");

    delete model;
    delete inverse;

}
