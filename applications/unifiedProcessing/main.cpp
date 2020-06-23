#include "chiIntegerVisualisation.h"
#include "cpuClock.h"
#include "createChiCSV.h"
#include "csvReader.h"
#include "factory.h"
#include "genericInputCardReader.h"
#include "inputCardReader.h"
#include "inversionInterface.h"
#include "log.h"
#include "utilityFunctions.h"
#include <string>

void performInversion(const io::genericInput &gInput, const std::string &runName, const std::string desired_inversion, const std::string desired_forward_model);
void writePlotInput(const io::genericInput &gInput, std::string msg);

int main(int argc, char **argv)
{
    if(argc != 4)
    {
        L_(io::linfo) << "Please give the case folder as argument. The case folder should contain an input and output folder." << std::endl;
        L_(io::linfo) << "Make sure the input folder inside the case folder contains the files genericInput.json, FMInput.json and CGInput.json" << std::endl;

        L_(io::linfo) << std::endl << "Please specify the desired inversion method" << std::endl;
        L_(io::linfo) << "Make sure the inversion method has been added as indicated in how_to_add_an_inversion_method.pdf" << std::endl;

        L_(io::linfo) << std::endl << "Please specify the desired forward model" << std::endl;
        L_(io::linfo) << "Make sure the forward model has been added as indicated in how_to_add_an_inversion_method.pdf" << std::endl;

        exit(EXIT_FAILURE);
    }

    try
    {
        std::vector<std::string> arguments(argv + 1, argc + argv);
        io::genericInputCardReader genericReader(arguments[0]);
        io::genericInput gInput = genericReader.getInput();
        std::string desired_inversion = arguments[1];
        std::string desired_forward_model = arguments[2];

        std::string logFileName = gInput.outputLocation + gInput.runName + "Process.log";

        if(!gInput.verbose)
        {
            std::cout << "Printing the program output onto a file named: " << logFileName << " in the output folder" << std::endl;
            initLogger(logFileName.c_str(), io::ldebug);
        }

        L_(io::linfo) << "Test";

        io::chi_visualisation_in_integer_form(gInput.inputFolder + gInput.fileName + ".txt", gInput.nGridOriginal[0]);
        createCsvFilesForChi(gInput.inputFolder + gInput.fileName + ".txt", gInput, "chi_reference_");

        performance::CpuClock clock;

        clock.Start();
        performInversion(gInput, gInput.runName, desired_inversion, desired_forward_model);
        clock.End();

        L_(io::linfo) << "Visualisation of the estimated temple using FWI";
        io::chi_visualisation_in_integer_form(gInput.outputLocation + "chi_est_" + gInput.runName + ".txt", gInput.nGrid[0]);
        createCsvFilesForChi(gInput.outputLocation + "chi_est_" + gInput.runName + ".txt", gInput, "chi_est_");

        std::string msg = clock.OutputString();
        writePlotInput(gInput, msg);
        io::endLogger();
    }
    catch(const std::invalid_argument &e)
    {
        std::cout << "An invalid argument found!" << std::endl;
        std::cout << e.what() << std::endl;
        L_(io::linfo) << "Invalid Argument Exception: " << e.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }
    catch(const std::exception &e)
    {
        std::cout << "An exception has been thrown:" << std::endl;
        std::cout << e.what() << std::endl;
        L_(io::linfo) << "Exception: " << e.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }

    return 0;
}

void writePlotInput(const io::genericInput &gInput, std::string msg)
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

void performInversion(const io::genericInput &gInput, const std::string &runName, const std::string desired_inversion, const std::string desired_forward_model)
{
    // initialize the grid sources receivers, grouped frequencies
    core::grid2D grid(gInput.reservoirTopLeftCornerInM, gInput.reservoirBottomRightCornerInM, gInput.nGrid);
    core::sources src(gInput.sourcesTopLeftCornerInM, gInput.sourcesBottomRightCornerInM, gInput.nSources);
    src.Print();
    core::receivers recv(gInput.receiversTopLeftCornerInM, gInput.receiversBottomRightCornerInM, gInput.nReceivers);
    recv.Print();
    core::frequenciesGroup freq(gInput.freq, gInput.c0);
    freq.Print(gInput.freq.nTotal);

    int magnitude = freq.nFreq * src.nSrc * recv.nRecv;

    // read referencePressureData from a CSV file format
    std::vector<std::complex<double>> referencePressureData(magnitude);

    std::string fileLocation = gInput.outputLocation + runName + "InvertedChiToPressure.txt";
    std::ifstream file(fileLocation);
    io::CSVReader row;

    if(!file.is_open())
    {
        L_(io::linfo) << "Could not open file at " << fileLocation;
        exit(EXIT_FAILURE);
    }

    int i = 0;
    L_(io::linfo) << "Read reference data" << fileLocation;
    while(file >> row)
    {
        if(i < magnitude)
        {
            referencePressureData[i] = {atof(row[0].c_str()), atof(row[1].c_str())};
        }
        i++;
    }

    Factory factory;

    L_(io::linfo) << "Create forwardModel";
    clock_t tStartForwardModel = clock();
    forwardModels::forwardModelInterface *model;
    model = factory.createForwardModel(gInput.caseFolder, desired_forward_model, grid, src, recv, freq);
    clock_t tEndForwardModel = clock();
    L_(io::linfo) << "Forwardmodel is created in " << double(tEndForwardModel - tStartForwardModel) / CLOCKS_PER_SEC << "seconds.";

    L_(io::linfo) << "Create inversionModel";
    clock_t tStartInversion = clock();
    inversionMethods::inversionInterface *inverse;
    inverse = factory.createInversion(desired_inversion, model, gInput);
    clock_t tEndInversion = clock();
    L_(io::linfo) << "Inversionmodel is created in " << double(tEndInversion - tStartInversion) / CLOCKS_PER_SEC << "seconds.";

    L_(io::linfo) << "Estimating Chi...";
    clock_t tStartEstimateChi = clock();
    core::dataGrid2D chi_est = inverse->reconstruct(referencePressureData, gInput);
    clock_t tEndEstimateChi = clock();
    L_(io::linfo) << "Estimated Chi in " << double(tEndEstimateChi - tStartEstimateChi) / CLOCKS_PER_SEC << "seconds.";
    L_(io::linfo) << "Writing to file";

    chi_est.toFile(gInput.outputLocation + "chi_est_" + runName + ".txt");
}
