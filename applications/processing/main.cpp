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
#include "integralForwardModel.h"

void performInversion(const genericInput& gInput, const forwardModelInput& fmInput, const conjugateGradientInput& cgInput, const std::string &runName);
void writePlotInput(const genericInput &gInput);

int main(int argc, char** argv)
{
    std::vector<std::string> arguments = returnInputDirectory(argc, argv);
    genericInputCardReader genericReader(arguments[0]);
    genericInput gInput = genericReader.getInput();

    forwardModelInputCardReader forwardModelReader(gInput.caseFolder);
    conjugateGradientInversionInputCardReader randomInversionReader(gInput.caseFolder);

    forwardModelInput fmInput = forwardModelReader.getInput();
    conjugateGradientInput cgInput = randomInversionReader.getInput();

    if (!gInput.verbose)
    {
        WriteToFileNotToTerminal(gInput.outputLocation, gInput.runName, "Process");
    }

    chi_visualisation_in_integer_form(gInput.inputFolder + gInput.fileName + ".txt", gInput.ngrid[0]);
    create_csv_files_for_chi(gInput.inputFolder + gInput.fileName + ".txt", gInput, "chi_reference_");

    cpuClock clock;

    clock.Start();
    performInversion(gInput, fmInput, cgInput, gInput.runName);
    clock.End();
    clock.PrintTimeElapsed();

    cout << "Visualisation of the estimated temple using FWI" << endl;
    chi_visualisation_in_integer_form(gInput.outputLocation + "chi_est_" + gInput.runName + ".txt", gInput.ngrid[0]);
    create_csv_files_for_chi(gInput.outputLocation + "chi_est_" + gInput.runName + ".txt", gInput, "chi_est_");

    writePlotInput(gInput);

    return 0;
}

void writePlotInput(const genericInput &gInput){
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
        lastrun.open(gInput.outputLocation + "/lastRunName.txt");
        lastrun << runName;
        lastrun.close();
}

void performInversion(const genericInput& gInput, const forwardModelInput& fmInput, const conjugateGradientInput& cgInput, const std::string &runName)
{
    // initialize the grid, sources, receivers, grouped frequencies
    grid2D grid(gInput.reservoirTopLeftCornerInM, gInput.reservoirBottomRightCornerInM, gInput.ngrid);
    sources src(gInput.sourcesTopLeftCornerInM, gInput.sourcesBottomRightCornerInM, gInput.nSourcesReceivers.src);
    src.Print();
    receivers recv(gInput.receiversTopLeftCornerInM, gInput.receiversBottomRightCornerInM, gInput.nSourcesReceivers.rec);
    recv.Print();
    frequenciesGroup freq(gInput.freq, gInput.c_0);
    freq.Print(gInput.freq.nTotal);

    int magnitude = freq.nFreq * src.nSrc * recv.nRecv;

    //read referencePressureData from a CSV file format
    std::complex<double> referencePressureData[magnitude];

    std::string fileLocation = gInput.outputLocation + runName + "InvertedChiToPressure.txt";
    std::ifstream       file(fileLocation);
    CSVReader           row;

    if(!file.is_open())
    {
        std::cout << "Could not open file at " << fileLocation;
        exit(EXIT_FAILURE);
    }

    int i = 0;
    while(file >> row)
    {
        if (i < magnitude)
        {
            referencePressureData[i]= { atof(row[0].c_str() ), atof(row[1].c_str()) };
        }
        i++;
    }

    ForwardModelInterface *model;
    model = new IntegralForwardModel(grid, src, recv, freq, fmInput);

    inversionInterface *inverse;
    inverse = new conjugateGradientInversion(model, cgInput);

    std::cout << "Estimating Chi..." << std::endl;

    pressureFieldSerial chi_est = inverse->Reconstruct(referencePressureData, gInput);

    std::cout << "Done, writing to file" << std::endl;

    chi_est.toFile(gInput.outputLocation + "chi_est_"+ runName + ".txt");

    delete model;
    delete inverse;
}
