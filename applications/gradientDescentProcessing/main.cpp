
#include "inputCardReader.h"
#include "genericInputCardReader.h"
#include "utilityFunctions.h"
#include "chiIntegerVisualisation.h"
#include "createChiCSV.h"
#include "csvReader.h"
#include "cpuClock.h"
#include "integralForwardModel.h"
#include "inversionInterface.h"
#include "gradientDescentInversion.h"

void performInversion(const GenericInput &gInput, const std::string &runName);
void writePlotInput(const GenericInput &gInput);

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "Please give the case folder as argument. The case folder should contain an input and output folder." << std::endl;
        std::cout << "Make sure the input folder inside the case folder contains the files GenericInput.json, ifmInput.json and GradientDescentInput.json" << std::endl;

        exit(EXIT_FAILURE);
    }

    std::vector<std::string> arguments(argv + 1, argc + argv);
    GenericInputCardReader genericReader(arguments[0]);
    GenericInput gInput = genericReader.getInput();

    //integralForwardModelInputCardReader forwardModelReader(gInput.caseFolder);
    //gradientDescentInversionInputCardReader GradientDescentInversionReader(gInput.caseFolder);

    //integralForwardModelInput ifmInput = forwardModelReader.getInput();
    //gradientDescentInversionInput gdInput = GradientDescentInversionReader.getInput();

    if (!gInput.verbose)
    {
        WriteToFileNotToTerminal(gInput.outputLocation, gInput.runName, "Process", -1);
    }

    chi_visualisation_in_integer_form(gInput.inputFolder + gInput.fileName + ".txt", gInput.ngrid[0]);
    create_csv_files_for_chi(gInput.inputFolder + gInput.fileName + ".txt", gInput, "chi_reference_");

    CpuClock clock;

    clock.Start();
    performInversion(gInput, gInput.runName);
    clock.End();
    clock.PrintTimeElapsed();

    std::cout << "Visualisation of the estimated temple using FWI" << std::endl;
    chi_visualisation_in_integer_form(gInput.outputLocation + "chi_est_" + gInput.runName + ".txt", gInput.ngrid[0]);
    create_csv_files_for_chi(gInput.outputLocation + "chi_est_" + gInput.runName + ".txt", gInput, "chi_est_");
    writePlotInput(gInput);

    return 0;
}

void writePlotInput(const GenericInput &gInput)
{
    // This part is needed for plotting the chi values in postProcessing.py
    std::ofstream outputfwi;
    std::string runName = gInput.runName;
    outputfwi.open(gInput.outputLocation + runName + ".pythonIn");
    outputfwi << "This run was parametrized as follows:" << std::endl;
    outputfwi << "nxt   = " << gInput.ngrid[0] << std::endl;
    outputfwi << "nzt   = " << gInput.ngrid[1] << std::endl;
    outputfwi << "nxt_original   = " << gInput.ngrid_original[0] << std::endl;
    outputfwi << "nzt_original   = " << gInput.ngrid_original[1] << std::endl;
    outputfwi.close();

    // This part is needed for plotting the chi values in postProcessing.py
    std::ofstream lastrun;
    lastrun.open(gInput.outputLocation + "/lastRunName.txt");
    lastrun << runName;
    lastrun.close();
}

void performInversion(const GenericInput &gInput, const std::string &runName)
{
    // initialize the grid, sources, receivers, grouped frequencies
    Grid2D grid(gInput.reservoirTopLeftCornerInM, gInput.reservoirBottomRightCornerInM, gInput.ngrid);
    Sources src(gInput.sourcesTopLeftCornerInM, gInput.sourcesBottomRightCornerInM, gInput.nSourcesReceivers.src);
    src.Print();
    Receivers recv(gInput.receiversTopLeftCornerInM, gInput.receiversBottomRightCornerInM, gInput.nSourcesReceivers.rec);
    recv.Print();
    FrequenciesGroup freq(gInput.freq, gInput.c_0);
    freq.Print(gInput.freq.nTotal);

    int magnitude = freq.nFreq * src.nSrc * recv.nRecv;

    //read referencePressureData from a CSV file format
    std::vector<std::complex<double>> referencePressureData(magnitude);

    std::string fileLocation = gInput.outputLocation + runName + "InvertedChiToPressure.txt";
    std::ifstream file(fileLocation);
    CSVReader row;

    if (!file.is_open())
    {
        std::cout << "Could not open file at " << fileLocation;
        exit(EXIT_FAILURE);
    }

    int i = 0;
    while (file >> row)
    {
        if (i < magnitude)
        {
            referencePressureData[i] = {atof(row[0].c_str()), atof(row[1].c_str())};
        }
        i++;
    }

    ForwardModelInterface *model;
    model = new IntegralForwardModel(grid, src, recv, freq, gInput);

    InversionInterface *inverse;
    inverse = new GradientDescentInversion(model, gInput);

    //    gradientDescentInversion(ForwardModelInterface *forwardModel, const GenericInput &gdInput);
    std::cout << "Estimating Chi..." << std::endl;

    PressureFieldSerial chi_est = inverse->Reconstruct(referencePressureData, gInput);

    std::cout << "Done, writing to file" << std::endl;

    chi_est.toFile(gInput.outputLocation + "chi_est_" + runName + ".txt");

    delete model;
    delete inverse;
}
