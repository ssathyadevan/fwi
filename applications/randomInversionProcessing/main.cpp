
#include "chiIntegerVisualisation.h"
#include "cpuClock.h"
#include "createChiCSV.h"
#include "csvReader.h"
#include "genericInputCardReader.h"
#include "inputCardReader.h"
#include "integralForwardModel.h"
#include "integralForwardModelInputCardReader.h"
#include "log.h"
#include "randomInversion.h"
#include "randomInversionInputCardReader.h"
#include "utilityFunctions.h"

void performInversion(const fwi::io::genericInput &gInput);
void writePlotInput(fwi::io::genericInput gInput, std::string msg);

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        L_(fwi::io::lerror) << "Please give the case folder as argument. The case folder should contain an input and output folder." << std::endl;
        L_(fwi::io::lerror)
            << "Make sure the input folder inside the case folder contains the files genericInput.json, FMInput.json and RandomInversionInput.json"
            << std::endl;

        exit(EXIT_FAILURE);
    }
    try
    {
        std::vector<std::string> arguments(argv + 1, argc + argv);
        fwi::io::genericInputCardReader genericReader(arguments[0]);
        fwi::io::genericInput gInput = genericReader.getInput();

        std::string logFileName = gInput.outputLocation + gInput.runName + "Process.log";

        if(!gInput.verbose)
        {
            std::cout << "Printing the program output onto a file named: " << logFileName << " in the output folder" << std::endl;
            fwi::io::initLogger(logFileName.c_str(), fwi::io::ldebug);
        }

        fwi::io::chi_visualisation_in_integer_form(gInput.inputFolder + gInput.fileName + ".txt", gInput.nGrid[0]);
        fwi::io::createCsvFilesForChi(gInput.inputFolder + gInput.fileName + ".txt", gInput, "chi_reference_");

        fwi::performance::CpuClock clock;

        clock.Start();
        performInversion(gInput);
        clock.End();

        L_(fwi::io::linfo) << "Visualisation of the estimated temple using FWI";
        fwi::io::chi_visualisation_in_integer_form(gInput.outputLocation + "chi_est_" + gInput.runName + ".txt", gInput.nGrid[0]);
        fwi::io::createCsvFilesForChi(gInput.outputLocation + "chi_est_" + gInput.runName + ".txt", gInput, "chi_est_");

        std::string msg = clock.OutputString();
        writePlotInput(gInput, msg);
    }
    catch(const std::invalid_argument &e)
    {
        std::cout << "An invalid argument found!" << std::endl;
        std::cout << e.what() << std::endl;
        L_(fwi::io::linfo) << "Invalid Argument Exception: " << e.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }
    catch(const std::exception &e)
    {
        std::cout << "An exception has been thrown:" << std::endl;
        std::cout << e.what() << std::endl;
        L_(fwi::io::linfo) << "Exception: " << e.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }

    return 0;
}

void writePlotInput(fwi::io::genericInput gInput, std::string msg)
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
    lastrun.open(gInput.outputLocation + "lastRunName.txt");
    lastrun << runName;
    lastrun.close();
}

void performInversion(const fwi::io::genericInput &gInput)
{
    // initialize the grid sources receivers, grouped frequencies
    fwi::core::grid2D grid(gInput.reservoirTopLeftCornerInM, gInput.reservoirBottomRightCornerInM, gInput.nGrid);
    fwi::core::Sources source(gInput.sourcesTopLeftCornerInM, gInput.sourcesBottomRightCornerInM, gInput.nSources);
    source.Print();
    fwi::core::Receivers receiver(gInput.receiversTopLeftCornerInM, gInput.receiversBottomRightCornerInM, gInput.nReceivers);
    receiver.Print();
    fwi::core::FrequenciesGroup freqg(gInput.freq, gInput.c0);
    freqg.Print(gInput.freq.nTotal);

    int magnitude = gInput.freq.nTotal * gInput.nSources * gInput.nReceivers;

    // read referencePressureData from a CSV file format
    std::vector<std::complex<double>> referencePressureData(magnitude);
    std::ifstream file(gInput.outputLocation + gInput.runName + "InvertedChiToPressure.txt");
    fwi::io::CSVReader row;
    int i = 0;
    while(file >> row)
    {
        if(i < magnitude)
        {
            referencePressureData[i] = {atof(row[0].c_str()), atof(row[1].c_str())};
        }
        i++;
    }

    L_(fwi::io::linfo) << "Create forwardModel";
    clock_t tStartForwardModel = clock();
    fwi::forwardModels::forwardModelInterface *model;
    fwi::forwardModels::integralForwardModelInputCardReader integralreader(gInput.caseFolder);
    model = new fwi::forwardModels::IntegralForwardModel(grid, source, receiver, freqg, integralreader.getInput());
    clock_t tEndForwardModel = clock();
    L_(fwi::io::linfo) << "Forwardmodel is created in " << double(tEndForwardModel - tStartForwardModel) / CLOCKS_PER_SEC << "seconds.";

    L_(fwi::io::linfo) << "Create inversionModel";
    clock_t tStartInversion = clock();
    fwi::inversionMethods::inversionInterface *inverse;
    fwi::inversionMethods::RandomInversionInputCardReader randomreader(gInput.caseFolder);
    inverse = new fwi::inversionMethods::RandomInversion(model, randomreader.getInput());
    clock_t tEndInversion = clock();
    L_(fwi::io::linfo) << "Inversionmodel is created in " << double(tEndInversion - tStartInversion) / CLOCKS_PER_SEC << "seconds.";

    L_(fwi::io::linfo) << "Estimating Chi...";
    clock_t tStartEstimateChi = clock();
    fwi::core::dataGrid2D chi_est = inverse->reconstruct(referencePressureData, gInput);
    clock_t tEndEstimateChi = clock();
    L_(fwi::io::linfo) << "Estimated Chi in " << double(tEndEstimateChi - tStartEstimateChi) / CLOCKS_PER_SEC << "seconds.";
    L_(fwi::io::linfo) << "Writing to file";

    chi_est.toFile(gInput.outputLocation + "chi_est_" + gInput.runName + ".txt");

    delete model;
    delete inverse;
}
