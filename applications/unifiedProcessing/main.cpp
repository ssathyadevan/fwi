#include "inversionFactory.h"
#include "inversionInterface.h"
#include "inputCardReader.h"
#include "genericInputCardReader.h"
#include "utilityFunctions.h"
#include "chiIntegerVisualisation.h"
#include "createChiCSV.h"
#include "csvReader.h"
#include "cpuClock.h"
#include "integralForwardModelInputCardReader.h"
#include "integralForwardModel.h"
#include <string>

void performInversion(const genericInput& gInput, const integralForwardModelInput& fmInput, const std::string &runName, const std::string desired_inversion);
void writePlotInput(const genericInput &gInput);

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::cout << "Please give the case folder as argument. The case folder should contain an input and output folder." << std::endl;
        std::cout << "Make sure the input folder inside the case folder contains the files GenericInput.json, FMInput.json and CGInput.json" << std::endl;
        
        std::cout << std::endl << "Please specify the desired inversion method" << std::endl;
        std::cout << "Make sure the inversion method has been added as indicated in how_to_add_an_inversion_method.pdf" << std::endl;

        exit(EXIT_FAILURE);
    }

    std::vector<std::string> arguments(argv+1, argc+argv);
    genericInputCardReader genericReader(arguments[0]);
    genericInput gInput = genericReader.getInput();
    std::string desired_inversion = arguments[1];

    integralForwardModelInputCardReader forwardModelReader(gInput.caseFolder);
    //conjugateGradientInversionInputCardReader conjugateGradientInversionReader(gInput.caseFolder);

    integralForwardModelInput fmInput = forwardModelReader.getInput();
    //conjugateGradientInput cgInput = conjugateGradientInversionReader.getInput();

    if (!gInput.verbose)
    {
        WriteToFileNotToTerminal(gInput.outputLocation, gInput.runName, "Process");
    }

    chi_visualisation_in_integer_form(gInput.inputFolder + gInput.fileName + ".txt", gInput.ngrid_original[0]);
    create_csv_files_for_chi(gInput.inputFolder + gInput.fileName + ".txt", gInput, "chi_reference_");

    cpuClock clock;

    clock.Start();
    performInversion(gInput, fmInput, gInput.runName, desired_inversion);
    clock.End();
    clock.PrintTimeElapsed();

    std::cout << "Visualisation of the estimated temple using FWI" << std::endl;
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
        outputfwi << "nxt_original   = " << gInput.ngrid_original[0]      << std::endl;
        outputfwi << "nzt_original   = " << gInput.ngrid_original[1]      << std::endl;
        outputfwi.close();

        // This part is needed for plotting the chi values in postProcessing.py
        std::ofstream lastrun;
        lastrun.open(gInput.outputLocation + "/lastRunName.txt");
        lastrun << runName;
        lastrun.close();
}

void performInversion(const genericInput& gInput, const integralForwardModelInput& fmInput, const std::string &runName, const std::string desired_inversion)
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
    inverse = inversionFactory::createInversion(desired_inversion, model, gInput);

    std::cout << "Estimating Chi..." << std::endl;

    pressureFieldSerial chi_est = inverse->Reconstruct(referencePressureData, gInput);

    std::cout << "Done, writing to file" << std::endl;

    chi_est.toFile(gInput.outputLocation + "chi_est_"+ runName + ".txt");

    delete model;
    delete inverse;
}
