#include <iostream>
#include <vector>

#include "log.h"
#include "genericInputCardReader.h"
#include "argumentReader.h"
#include "cpuClock.h"
#include "factory.h"
#include "StepAndDirectionReconstructorInputCardReader.h"
#include "chiIntegerVisualisation.h"
#include "createChiCSV.h"
#include "csvReader.h"
#include "HelpTextProcessing.h"

void printHelpOrVersion(fwi::io::argumentReader& fwiOpts);
void executeFullFWI(const fwi::io::argumentReader& fwiOpts);
void doProcess(const fwi::io::argumentReader& fwiOpts, const fwi::io::genericInput& gInput);
void writePlotInput(const fwi::io::genericInput &gInput, std::string msg);

int main(int argc, char* argv[])
{
   
    try
    {
        std::vector<std::string> arguments = {argv + 1, argv + argc};
        fwi::io::argumentReader fwiOpts(arguments);
        printHelpOrVersion(fwiOpts);
        
        fwi::io::genericInputCardReader genericReader(fwiOpts.dir);
        const fwi::io::genericInput gInput = genericReader.getInput();
        doProcess(fwiOpts, gInput);

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        std::cout << "Use -h for help on parameter options and values." << std::endl;
        L_(fwi::io::lerror) << e.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

void printHelpOrVersion(fwi::io::argumentReader& fwiOpts)
{
    if(fwiOpts.help)
    {
        std::cout << HELP_TEXT_PROCESSING << std::endl;
        std::exit(EXIT_SUCCESS);
    }

    if(fwiOpts.version)
    {
        std::cout << VERSION_PROCESSING << std::endl;
        std::exit(EXIT_SUCCESS);
    }
}

void doProcess(const fwi::io::argumentReader& fwiOpts, const fwi::io::genericInput& gInput)
{
    std::cout << "Inversion Processing Started" << std::endl; 

    // initialize the clock, grid sources receivers, grouped frequencies
    fwi::performance::CpuClock clock;
    fwi::core::grid2D grid(gInput.reservoirTopLeftCornerInM, gInput.reservoirBottomRightCornerInM, gInput.nGrid);
    fwi::core::Sources source(gInput.sourcesTopLeftCornerInM, gInput.sourcesBottomRightCornerInM, gInput.nSources);
    fwi::core::Receivers receiver(gInput.receiversTopLeftCornerInM, gInput.receiversBottomRightCornerInM, gInput.nReceivers);
    fwi::core::FrequenciesGroup freq(gInput.freq, gInput.c0);

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
    model = factory.createForwardModel(gInput.caseFolder, fwiOpts.forward + "ForwardModel", grid, source, receiver, freq);

    L_(fwi::io::linfo) << "Create inversionModel";
    fwi::inversionMethods::inversionInterface* inverse;

    std::cout << "Creating InversionProcess using parameters:" << std::endl 
            << "  -f=" << fwiOpts.forward << std::endl
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
        inverse = factory.createInversion(fwiOpts.inversion + "Inversion", model, gInput);
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