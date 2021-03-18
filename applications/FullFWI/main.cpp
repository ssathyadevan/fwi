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

const std::string HELP_TEXT = "This is the help message.";
const std::string VERSION = "Version 0.1 - 15/03/2021";

std::vector<std::string> readArguments(int argc, char* argv[]);
void printHelpOrVersion(const std::string& firstArg);
void executeFullFWI(const FullFWIOptions& fwiOpts);
void writePlotInput(const fwi::io::genericInput &gInput, std::string msg);

int main(int argc, char* argv[])
{
    try
    {
        std::vector<std::string> arguments = readArguments(argc, argv);
        printHelpOrVersion(arguments[0]);
        FullFWIOptions fwiOpts(arguments);
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
    /********************************
     Initialize
    *********************************/
    std::cout << "Initializing Full FWI program" << std::endl;
    fwi::performance::CpuClock clock;

    // read input json files
    fwi::io::genericInputCardReader genericReader(fwiOpts.dir);
    const fwi::io::genericInput gInput = genericReader.getInput();

    // initialize the grid sources receivers, grouped frequencies
    fwi::core::grid2D grid(gInput.reservoirTopLeftCornerInM, gInput.reservoirBottomRightCornerInM, gInput.nGrid);
    fwi::core::Sources source(gInput.sourcesTopLeftCornerInM, gInput.sourcesBottomRightCornerInM, gInput.nSources);
    fwi::core::Receivers receiver(gInput.receiversTopLeftCornerInM, gInput.receiversBottomRightCornerInM, gInput.nReceivers);
    fwi::core::FrequenciesGroup freq(gInput.freq, gInput.c0);

    /********************************
     Do preprocessing
    *********************************/
    if(!fwiOpts.skippre)
    {
        std::cout << "Starting preprocessing with parameter\n  -p=" + fwiOpts.preprocess << std::endl;

        // Initialize logger
        std::string logFilePath = gInput.outputLocation + gInput.runName + "PreProcess.log";
        if(!gInput.verbose)
        {
            std::cout << "Printing the preprocess output into file: " << logFilePath << std::endl;
            fwi::io::initLogger(logFilePath.c_str(), fwi::io::ldebug);
            
        }

        // Logging the setup
        L_(fwi::io::linfo) << "Starting PreProcess";
        source.Print();
        receiver.Print();
        freq.Print(gInput.freq.nTotal);

        // Create Model
        L_(fwi::io::linfo) << "Create forwardModel";
        
        clock.Start();
        fwi::Factory factory;
        fwi::forwardModels::ForwardModelInterface *model = factory.createForwardModel(
            gInput.caseFolder, fwiOpts.solver + "ForwardModel", grid, source, receiver, freq);
        clock.End();
        clock.OutputString();

        // Read chi from file and write to output file
        fwi::core::dataGrid2D chi(grid);
        std::string inputPath = gInput.inputFolder + gInput.fileName + ".txt";
        chi.fromFile(inputPath);
        std::string outputPath = gInput.outputLocation + "chi_ref_" + gInput.runName + ".txt";
        chi.toFile(outputPath);

        // Calculate pressure field data
        std::cout << "Calculating..." << std::endl;
        L_(fwi::io::linfo) << "Calculating pData (the reference pressure-field)...";
        clock.Start();
        model->calculatePTot(chi);
        model->calculateKappa();
        std::vector<std::complex<double>> referencePressureData = model->calculatePressureField(chi);
        clock.End();
        clock.OutputString();
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
        fwi::io::endLogger();
        std::cout << "Preprocessing completed" << std::endl;
    }
    else 
        std::cout << "Preprocessing skipped" << std::endl;        


    /********************************
     Do inversion
    *********************************/

    std::cout << "Inversion Processing Started" << std::endl; 

    //initialize logging
    std::string logFileName = gInput.outputLocation + gInput.runName + "Process.log";

    if(!gInput.verbose)
    {
        std::cout << "Printing the program output onto a file named: " << logFileName << std::endl;
        fwi::io::initLogger(logFileName.c_str(), fwi::io::ldebug);
    }

    // Logging the setup
    L_(fwi::io::linfo) << "Starting Inversion Process";
    source.Print();
    receiver.Print();
    freq.Print(gInput.freq.nTotal);

    // Logging chi from input
    L_(fwi::io::linfo) << "Visualisation of input chi (to be reconstructed)";
    fwi::io::chi_visualisation_in_integer_form(gInput.inputFolder + gInput.fileName + ".txt", gInput.nGridOriginal[0]);
    fwi::io::createCsvFilesForChi(gInput.inputFolder + gInput.fileName + ".txt", gInput, "chi_reference_");

    
    // Start inversion
    clock.Start();
    
    // read referencePressureData from a CSV file format
    std::string fileLocation = gInput.outputLocation + gInput.runName + "InvertedChiToPressure.txt";
    std::ifstream file(fileLocation);
    fwi::io::CSVReader row;

    if(!file.is_open())
    {
        L_(fwi::io::linfo) << "Could not open file at " << fileLocation;
        throw std::runtime_error("Could not open file at " + fileLocation);
    }

    int magnitude = freq.count * source.count * receiver.count;
    std::vector<std::complex<double>> referencePressureData(magnitude);
    int i = 0;
    L_(fwi::io::linfo) << "Read reference data" << fileLocation;
    while(file >> row)
    {
        if(i < magnitude)
        {
            referencePressureData[i] = {atof(row[0].c_str()), atof(row[1].c_str())};
        }
        ++i;
    }

    // Create model
    fwi::Factory factory;
    L_(fwi::io::linfo) << "Create ForwardModel";
    fwi::forwardModels::ForwardModelInterface *model;
    model = factory.createForwardModel(gInput.caseFolder, fwiOpts.solver + "ForwardModel", grid, source, receiver, freq);

    L_(fwi::io::linfo) << "Create inversionModel";
    fwi::inversionMethods::inversionInterface* inverse;

    std::cout << "Creating InversionProcess using parameters:" << std::endl 
              << "  -s=" << fwiOpts.solver << std::endl
              << "  -i=" << fwiOpts.inversion << std::endl;

    // Setup StepAndDirection or UnifiedProcess models
    if(fwiOpts.inversion == "StepAndDirection")
    {
        std::cout << "  --stepdir="  << fwiOpts.stepdir << std::endl
                  << "  --stepsize=" << fwiOpts.stepsize << std::endl;

        L_(fwi::io::linfo) << "Create StepAndDirectionReconstructor";
        fwi::inversionMethods::StepAndDirectionReconstructorInputCardReader stepAndDirectionReader(gInput.caseFolder);
        fwi::inversionMethods::StepAndDirectionReconstructorInput stepAndDirectionInput = stepAndDirectionReader.getInput();
        inverse = factory.createStepAndDirectionReconstructor(stepAndDirectionInput, model, 
                                                              fwiOpts.stepsize + "StepSize", fwiOpts.stepdir + "Direction", referencePressureData);  
    }
    else
    {
        L_(fwi::io::linfo) << "Create UnifiedInversionReconstructor";
        inverse = factory.createInversion(fwiOpts.inversion, model, gInput);
    }

    std::cout << "Calculating..." << std::endl;
    L_(fwi::io::linfo) << "Estimating Chi...";
    fwi::core::dataGrid2D chiEstimate = inverse->reconstruct(referencePressureData, gInput);
    
    L_(fwi::io::linfo) << "Writing to file";
    chiEstimate.toFile(gInput.outputLocation + "chi_est_" + gInput.runName + ".txt");

    clock.End();

    L_(fwi::io::linfo) << "Visualisation of the estimated chi using FWI";
    fwi::io::chi_visualisation_in_integer_form(gInput.outputLocation + "chi_est_" + gInput.runName + ".txt", gInput.nGrid[0]);
    fwi::io::createCsvFilesForChi(gInput.outputLocation + "chi_est_" + gInput.runName + ".txt", gInput, "chi_est_");

    std::string msg = clock.OutputString();
    writePlotInput(gInput, msg);
    fwi::io::endLogger();

    std::cout << "InversionProcess completed" << std::endl;

    /********************************
     PostProcessing
    *********************************/

}

void writePlotInput(const fwi::io::genericInput &gInput, std::string msg)
{
    // This part is needed for plotting the chi values in postProcessing.py
    std::ofstream outputfwi;
    std::string runName = gInput.runName;
    outputfwi.open(gInput.outputLocation + runName + ".pythonIn");
    outputfwi << "This run was parametrized as follows:" << std::endl;
    outputfwi << "nxt   = " << gInput.nGrid[0] << std::endl;
    outputfwi << "nzt   = " << gInput.nGrid[1] << std::endl;
    outputfwi << "nxt_original   = " << gInput.nGridOriginal[0] << std::endl;
    outputfwi << "nzt_original   = " << gInput.nGridOriginal[1] << std::endl << msg;
    outputfwi.close();

    // This part is needed for plotting the chi values in postProcessing.py
    std::ofstream lastrun;
    lastrun.open(gInput.outputLocation + "/lastRunName.txt");
    lastrun << runName;
    lastrun.close();
}