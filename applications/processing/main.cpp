#include "conjugateGradientInversion.h"
#include "inputCardReader.h"
#include "genericInputCardReader.h"
#include "conjugateGradientInversionInputCardReader.h"
#include "utilityFunctions.h"
#include "chiIntegerVisualisation.h"
#include "createChiCSV.h"
#include "csvReader.h"
#include "cpuClock.h"
#include "forwardModelInputCardReader.h"

void performInversion(const genericInput& gInput, const forwardModelInput& fmInput, const conjugateGradientInput& cgInput, const std::string &runName);
void writePlotInput(const genericInput &gInput, const std::string &outputLocation);

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::cout << "Please enter 2 arguments, 1st the location of the input card set, 2nd the output folder location," << std::endl;
        std::cout << "Make sure the input folder contains the files GenericInput.in, ForwardModelInput.in and ConjugateGradientInversionInput" << std::endl;
        std::cout << "e.g. ../input/default/ ../output/" << std::endl;

        exit(EXIT_FAILURE);
    }
    std::vector<std::string> arguments(argv+1, argc+argv);

    std::vector<std::string> arguments(argv+1, argc+argv);    
    std::string inputFolder  = arguments[0];
    std::string outputFolder = arguments[1];

    std::string runName = determineRunName(inputFolder);

    genericInputCardReader genericReader(inputFolder, outputFolder, runName);
    forwardModelInputCardReader forwardModelReader(inputFolder);
    conjugateGradientInversionInputCardReader conjugateGradientReader(inputFolder);

    genericInput           gInput  = genericReader.getInput();
    forwardModelInput      fmInput = forwardModelReader.getInput();
    conjugateGradientInput cgInput = conjugateGradientReader.getInput();

    if (!gInput.verbose)
    {
        WriteToFileNotToTerminal(gInput.outputLocation, runName, "Process");
    }

    chi_visualisation_in_integer_form(gInput.inputCardPath + gInput.fileName + ".txt", gInput.ngrid[0]);
    create_csv_files_for_chi(gInput.inputCardPath + gInput.fileName + ".txt", gInput, "chi_reference_");

    cpuClock clock;

    clock.Start();
    performInversion(gInput, fmInput, cgInput, runName);
    clock.End();
    clock.PrintTimeElapsed();

    cout << "Visualisation of the estimated temple using FWI" << endl;
    chi_visualisation_in_integer_form(gInput.outputLocation + "chi_est_" + gInput.runName + ".txt", gInput.ngrid[0]);
    create_csv_files_for_chi(gInput.outputLocation + "chi_est_" + runName + ".txt", gInput, "chi_est_");

    writePlotInput(gInput, outputFolder);

    return 0;
}

void writePlotInput(const genericInput &gInput, const std::string &outputLocation){
        // This part is needed for plotting the chi values in postProcessing.py
        std::ofstream outputfwi;
        std::string runName = gInput.runName;
        outputfwi.open(outputLocation + runName + ".pythonIn");
        outputfwi << "This run was parametrized as follows:" << std::endl;
        outputfwi << "nxt   = " << gInput.ngrid[0]      << std::endl;
        outputfwi << "nzt   = " << gInput.ngrid[1]      << std::endl;
        outputfwi.close();

        // This part is needed for plotting the chi values in postProcessing.py
        std::ofstream lastrun;
        lastrun.open(outputLocation + "/lastRunName.txt");
        lastrun << runName;
        lastrun.close();
}

void performInversion(const genericInput& gInput, const forwardModelInput& fmInput, const conjugateGradientInput& cgInput, const std::string &runName)
{
    // initialize the grid, sources, receivers, grouped frequencies
    grid2D grid(gInput.reservoirTopLeftCornerInM, gInput.reservoirBottomRightCornerInM, gInput.ngrid);
    sources src(gInput.sourcesTopLeftCornerInM, gInput.sourcesBottomRightCornerInM, gInput.nSourcesReceivers.src);
    src.Print();
    receivers recv(input.receiversTopLeftCornerInM, input.receiversBottomRightCornerInM, input.nSourcesReceivers.rec);
    recv.Print();
    frequenciesGroup freqg(gInput.freq, gInput.c_0);
    freqg.Print(gInput.freq.nTotal);

    int magnitude = gInput.freq.nTotal * gInput.nSourcesReceivers.src * gInput.nSourcesReceivers.rec;
    //read referencePressureData from a CSV file format
    std::complex<double> referencePressureData[magnitude];
    std::ifstream       file( gInput.outputLocation + runName + "InvertedChiToPressure.txt");
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
    inverse = new conjugateGradientInversion(model,cgInput);

    std::cout << "Estimating Chi..." << std::endl;

    pressureFieldSerial chi_est = inverse->Reconstruct(referencePressureData, gInput);

    std::cout << "Done, writing to file" << std::endl;

    chi_est.toFile(gInput.outputLocation + "chi_est_"+ runName + ".txt");

    delete model;
    delete inverse;

}
