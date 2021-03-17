#include <iostream>
#include <vector>

#include "log.h"
#include "genericInputCardReader.h"
#include "FullFWIOptions.h"
#include "cpuClock.h"
#include "factory.h"
#include "integralForwardModel.h"
#include "integralForwardModelInputCardReader.h"
#include "FiniteDifferenceForwardModel.h"
#include "FiniteDifferenceForwardModelInputCardReader.h"
#include "utilityFunctions.h"
#include "DirectionCalculator.h"
#include "StepAndDirectionReconstructor.h"
#include "StepAndDirectionReconstructorInputCardReader.h"
#include "StepSizeCalculator.h"
#include "chiIntegerVisualisation.h"
#include "createChiCSV.h"
#include "csvReader.h"
#include "inputCardReader.h"
#include "inversionInterface.h"




// Unused?
const std::vector<std::string> POSSIBLE_ARGS = {
    "-d", "--dir"
    "-p", "--preprocess",
    "-s", "--solver",
    "-i", "--inversion",
    "--stepdir",
    "--stepsize",
    "--skip-post",
    "--skip-pre"
};

const std::string HELP_TEXT = "This is the help message.";

const std::string VERSION = "Version 0.1 - 15/03/2021";

std::vector<std::string> readArguments(int argc, char* argv[]);
void printHelpOrVersion(const std::string& firstArg);
void executeFullFWI(const FullFWIOptions& fwiOpts);

int main(int argc, char* argv[])
{
    try
    {
        std::vector<std::string> arguments = readArguments(argc, argv);
        printHelpOrVersion(arguments[0]);
        FullFWIOptions fwiOpts(arguments);

        // Dummy code: Print all options as read from the input arguments
        std::cout << "dir=" << fwiOpts.dir << std::endl;
        std::cout << "preprocess=" << fwiOpts.preprocess << std::endl;
        std::cout << "solver=" << fwiOpts.solver << std::endl;
        std::cout << "inversion=" << fwiOpts.inversion << std::endl;
        std::cout << "stepdir=" << fwiOpts.stepdir << std::endl;
        std::cout << "stepsize=" << fwiOpts.stepsize << std::endl;
        std::cout << "skippost=" << fwiOpts.skippost << std::endl;
        std::cout << "skippre=" << fwiOpts.skippre << std::endl;

        executeFullFWI(fwiOpts);
            
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        std::cout << "Use -h for help on parameter options and values." << std::endl;
        L_(fwi::io::lerror) << e.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

std::vector<std::string> readArguments(int argc, char* argv[])
{
    // First argument is always the executable location, we can ignore that.
    if (argc == 1)
        throw std::invalid_argument(
            "No arguments received. Program requires -d or --dir flag specifying the path to the input and output folders."
        );
    std::vector<std::string> arguments(argv + 1, argv + argc);
    
    // Append empty entry to avoid looking for flag option value beyond set arguments.
    arguments.push_back("");
    return arguments;
}

void printHelpOrVersion(const std::string& firstArg)
{
    if(firstArg == "-h" or firstArg == "--help")
    {
        std::cout << HELP_TEXT << std::endl;
        std::exit(EXIT_SUCCESS);
    }

    if(firstArg == "-v" or firstArg == "--version")
    {
        std::cout << VERSION << std::endl;
        std::exit(EXIT_SUCCESS);
    }
}

void executeFullFWI(const FullFWIOptions& fwiOpts)
{
    // read input json files
    fwi::io::genericInputCardReader genericReader(fwiOpts.dir);
    const fwi::io::genericInput gInput = genericReader.getInput();

    // Initialize logger
    std::string logFilePath = gInput.outputLocation + gInput.runName + "PreProcess.log";
    if(!gInput.verbose)
        {
            std::cout << "Printing the program output into" << logFilePath << std::endl;
            fwi::io::initLogger(logFilePath.c_str(), fwi::io::ldebug);
        }

    // initialize the grid sources receivers, grouped frequencies
    fwi::core::grid2D grid(gInput.reservoirTopLeftCornerInM, gInput.reservoirBottomRightCornerInM, gInput.nGrid);

    fwi::core::dataGrid2D chi(grid);
    std::string inputPath = gInput.inputFolder + gInput.fileName + ".txt";
    chi.fromFile(inputPath);
    std::string outputPath = gInput.outputLocation + "chi_ref_" + gInput.runName + ".txt";
    chi.toFile(outputPath);

    fwi::core::Sources source(gInput.sourcesTopLeftCornerInM, gInput.sourcesBottomRightCornerInM, gInput.nSources);
    source.Print();
    fwi::core::Receivers receiver(gInput.receiversTopLeftCornerInM, gInput.receiversBottomRightCornerInM, gInput.nReceivers);
    receiver.Print();
    fwi::core::FrequenciesGroup freq(gInput.freq, gInput.c0);
    freq.Print(gInput.freq.nTotal);
    

    // Do preprocessing
    if(!fwiOpts.skippre)
    {
        // Create Model
        L_(fwi::io::linfo) << "Create forwardModel";
        clock_t tStartForwardModel = clock();
        fwi::Factory factory;
        fwi::forwardModels::ForwardModelInterface *model = factory.createForwardModel(
            gInput.caseFolder, fwiOpts.solver + "ForwardModel", grid, source, receiver, freq);
        clock_t tEndForwardModel = clock();
        L_(fwi::io::linfo) << "Forwardmodel is created in " << double(tEndForwardModel - tStartForwardModel) / CLOCKS_PER_SEC << " seconds.";



        // Calculate pressure field data
        L_(fwi::io::linfo) << "Calculating pData (the reference pressure-field)...";
        clock_t tStartCalculatePData = clock();
        model->calculatePTot(chi);
        model->calculateKappa();
        std::vector<std::complex<double>> referencePressureData = model->calculatePressureField(chi);
        clock_t tEndCalculatePData = clock();
        L_(fwi::io::linfo) << "PData has been calculated in " << double(tEndCalculatePData - tStartCalculatePData) / CLOCKS_PER_SEC << " seconds.";
        L_(fwi::io::linfo) << "calculateData done";



        // writing the referencePressureData to a text file in complex form
        std::string invertedChiToPressureFileName = gInput.outputLocation + gInput.runName + "InvertedChiToPressure.txt";
        L_(fwi::io::linfo) << "Writing pData to file " << invertedChiToPressureFileName;
        std::ofstream file;
        file.open(invertedChiToPressureFileName, std::ios::out | std::ios::trunc);

        if(!file)
        {
            L_(fwi::io::lerror) << "Failed to open the file to store inverted chi to pressure field";
            throw std::runtime_error("Could not open file at " + invertedChiToPressureFileName);
        }

        int magnitude = freq.count * source.count * receiver.count;
        for(int i = 0; i < magnitude; i++)
        {
            file << std::setprecision(17) << referencePressureData[i].real() << "," << referencePressureData[i].imag() << std::endl;
        }
        file.close();
        L_(fwi::io::linfo) << "PreProcessing Task finished successfully!";
    }


}