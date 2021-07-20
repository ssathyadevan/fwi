#include "HelpTextProcessing.h"
#include "FiniteDifferenceForwardModel.h"
#include "FiniteDifferenceForwardModelInputCardReader.h"
#include "FiniteDifferenceForwardModelParallelMPI.h"
#include "argumentReader.h"
#include "chiIntegerVisualisation.h"
#include "cpuClock.h"
#include "createChiCSV.h"
#include "csvReader.h"
#include "factory.h"
#include "genericInputCardReader.h"
#include "log.h"
#include <boost/mpi.hpp>
#include <iostream>
#include <mpi.h>
#include <vector>
namespace mpi = boost::mpi;

void printHelpOrVersion(fwi::io::argumentReader &fwiOpts);
void executeFullFWI(const fwi::io::argumentReader &fwiOpts);
void doProcess(const fwi::io::genericInput &gInput);
void doProcessMPI( const fwi::io::genericInput &gInput);
void writePlotInput(const fwi::io::genericInput &gInput, std::string msg);

int main(int argc, char *argv[])
{
    try
    {
        mpi::environment env(argc, argv, mpi::threading::multiple);
        mpi::communicator world;

        std::vector<std::string> arguments = {argv + 1, argv + argc};
        fwi::io::argumentReader fwiOpts(arguments);
        printHelpOrVersion(fwiOpts);

        fwi::io::genericInputCardReader genericReader(fwiOpts);
        const fwi::io::genericInput gInput = genericReader.getInput();

        if(world.rank() == 0)
        {
            printf("Started Process with %i threads \n", world.size());
            doProcess(gInput);
            env.abort(1);
        }
        else
        {
            doProcessMPI(gInput);
        }
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        std::cout << "Use -h for help on parameter options and values." << std::endl;
        L_(fwi::io::lerror) << e.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

void printHelpOrVersion(fwi::io::argumentReader &fwiOpts)
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

void doProcess(const fwi::io::genericInput &gInput)
{
    std::cout << "Inversion Processing Started" << std::endl;

    // initialize the clock, grid sources receivers, grouped frequencies
    fwi::performance::CpuClock clock;
    fwi::core::grid2D grid(gInput.reservoirTopLeftCornerInM, gInput.reservoirBottomRightCornerInM, gInput.nGrid);
    fwi::core::Sources source(gInput.sourcesTopLeftCornerInM, gInput.sourcesBottomRightCornerInM, gInput.nSources);
    fwi::core::Receivers receiver(gInput.receiversTopLeftCornerInM, gInput.receiversBottomRightCornerInM, gInput.nReceivers);
    fwi::core::FrequenciesGroup freq(gInput.freq, gInput.c0);

    // initialize logging
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
    model = factory.createForwardModel(gInput.caseFolder, gInput.forward + "ForwardModel", grid, source, receiver, freq);

    L_(fwi::io::linfo) << "Create inversionModel";
    fwi::inversionMethods::inversionInterface *inverse;

    std::cout << "Creating InversionProcess using parameters:" << std::endl
              << "  -f=" << gInput.forward << std::endl
              << "  -i=" << gInput.inversion << std::endl;

    // Setup StepAndDirection or UnifiedProcess models
    inverse = factory.createInversion(gInput.inversion + "Inversion", model, gInput);

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

void doProcessMPI(const fwi::io::genericInput &gInput)
{
    mpi::environment env;
    mpi::communicator world;
    fwi::core::grid2D grid(gInput.reservoirTopLeftCornerInM, gInput.reservoirBottomRightCornerInM, gInput.nGrid);
    fwi::core::Sources source(gInput.sourcesTopLeftCornerInM, gInput.sourcesBottomRightCornerInM, gInput.nSources);
    fwi::core::Receivers receiver(gInput.receiversTopLeftCornerInM, gInput.receiversBottomRightCornerInM, gInput.nReceivers);
    fwi::core::FrequenciesGroup freq(gInput.freq, gInput.c0);

    // read referencePressureData from a CSV file format
    std::string fileLocation = gInput.outputLocation + gInput.runName + "InvertedChiToPressure.txt";
    std::ifstream file(fileLocation);
    fwi::io::CSVReader row;


    if(!file.is_open())
    {
        //L_(fwi::io::linfo) << "Could not open file at " << fileLocation;
        throw std::runtime_error("Could not open file at " + fileLocation);
    }

    int magnitude = freq.count * source.count * receiver.count;
    std::vector<std::complex<double>> referencePressureData(magnitude);
    int i = 0;

    while(file >> row)
    {
        if(i < magnitude)
        {
            referencePressureData[i] = {atof(row[0].c_str()), atof(row[1].c_str())};
        }
        ++i;
    }

    // Create forward model
    fwi::Factory factory;
    L_(fwi::io::linfo) << "Create ForwardModel";
    fwi::forwardModels::ForwardModelInterface *model;
    model = factory.createForwardModel(gInput.caseFolder, gInput.forward + "ForwardModel", grid, source, receiver, freq);

    std::vector<std::complex<double>> res;
    fwi::core::complexDataGrid2D cgrid(grid);

    bool loop = true;
    while(loop)
    {
       model->getUpdateDirectionInformationMPI(res, cgrid,2,5);
    }

    printf("Rank %i has broken from loop with buffer %d \n", world.rank(), loop);
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