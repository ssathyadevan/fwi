
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
void writePlotInput(genericInput gInput);

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "Please give the case folder as argument. The case folder should contain an input and output folder." << std::endl;
        std::cout << "Make sure the input folder inside the case folder contains the files GenericInput.json, FMInput.json and RandomInversionInput.json" << std::endl;

        exit(EXIT_FAILURE);
    }

    std::vector<std::string> arguments(argv+1, argc+argv);
    genericInputCardReader genericReader(arguments[0]);
    genericInput gInput = genericReader.getInput();

    forwardModelInputCardReader forwardModelReader(gInput.caseFolder);
    randomInversionInputCardReader randomInversionReader(gInput.caseFolder);

    forwardModelInput fmInput = forwardModelReader.getInput();
    randomInversionInput riInput = randomInversionReader.getInput();

    if (!gInput.verbose)
    {
        WriteToFileNotToTerminal(gInput.outputLocation, gInput.runName, "Process");
    }

    chi_visualisation_in_integer_form(gInput.inputFolder+ gInput.fileName + ".txt", gInput.ngrid[0]);
    create_csv_files_for_chi(gInput.inputFolder + gInput.fileName + ".txt", gInput, "chi_reference_");

    cpuClock clock;

    clock.Start();
    performInversion(gInput, fmInput, riInput);
    clock.End();
    clock.PrintTimeElapsed();

    cout << "Visualisation of the estimated temple using FWI" << endl;
    chi_visualisation_in_integer_form(gInput.outputLocation + "chi_est_" + gInput.runName + ".txt", gInput.ngrid[0]);
    create_csv_files_for_chi(gInput.outputLocation + "chi_est_" + gInput.runName + ".txt", gInput, "chi_est_");

    writePlotInput(gInput);

    return 0;
}

void writePlotInput(genericInput gInput){
        // This part is needed for plotting the chi values in postProcessing.py
        std::ofstream outputfwi;
        std::string runName = gInput.runName;
        outputfwi.open(gInput.outputLocation + runName + ".pythonIn");
        outputfwi << "This run was parametrized as follows:" << std::endl;
        outputfwi << "nxt   = " << gInput.ngrid[0]      << std::endl;
        outputfwi << "nzt   = " << gInput.ngrid[1]      << std::endl;
        outputfwi.close();

        // This part is needed for plotting the chi values in postProcessing.py
        std::ofstream lastrun;
        lastrun.open(gInput.outputLocation + "lastRunName.txt");
        lastrun << runName;
        lastrun.close();
}

void performInversion(const genericInput &gInput, const forwardModelInput &fmInput, const randomInversionInput &riInput)
{
    // initialize the grid, sources, receivers, grouped frequencies
    grid2D grid(gInput.reservoirTopLeftCornerInM, gInput.reservoirBottomRightCornerInM, gInput.ngrid);
    sources src(gInput.sourcesTopLeftCornerInM, gInput.sourcesBottomRightCornerInM, gInput.nSourcesReceivers.src);
    src.Print();
    receivers recv(gInput.receiversTopLeftCornerInM, gInput.receiversBottomRightCornerInM, gInput.nSourcesReceivers.rec);
    recv.Print();
    frequenciesGroup freqg(gInput.freq, gInput.c_0);
    freqg.Print(gInput.freq.nTotal);

    int magnitude = gInput.freq.nTotal * gInput.nSourcesReceivers.src * gInput.nSourcesReceivers.rec;

    //read referencePressureData from a CSV file format
    std::complex<double> referencePressureData[magnitude];
    std::ifstream       file(gInput.outputLocation+gInput.runName+"InvertedChiToPressure.txt");
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
    model = new IntegralForwardModel(grid, src, recv, freqg, fmInput);

    inversionInterface *inverse;
    inverse = new inversionRandom(model, riInput);

    std::cout << "Estimating Chi..." << std::endl;

    pressureFieldSerial chi_est = inverse->Reconstruct(referencePressureData, gInput);

    std::cout << "Done, writing to file" << std::endl;

    chi_est.toFile(gInput.outputLocation + "chi_est_"+ gInput.runName+ ".txt");

    delete model;
    delete inverse;
}
