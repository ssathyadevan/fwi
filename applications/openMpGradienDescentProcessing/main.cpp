#include "OpenMPgradientDescentInversion.h"
#include "chiIntegerVisualisation.h"
#include "cpuClock.h"
#include "createChiCSV.h"
#include "csvReader.h"
#include "forwardModelContainer.h"
#include "genericInputCardReader.h"
#include "inputCardReader.h"
#include "integralForwardModel.h"
#include "integralForwardModelInputCardReader.h"
#include "inversionInterface.h"
#include "log.h"
#include "utilityFunctions.h"
#include <string>

void performInversion(const fwi::io::genericInput &gInput, const std::string &runName);
void writePlotInput(const fwi::io::genericInput &gInput, std::string msg);

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        L_(fwi::io::linfo) << "Please give the case folder as argument. The case folder should contain an input and output folder." << std::endl;
        L_(fwi::io::linfo) << "Make sure the input folder inside the case folder contains the files genericInput.json, FMInput.json and CGInput.json"
                           << std::endl;

        exit(EXIT_FAILURE);
    }

    std::vector<std::string> arguments(argv + 1, argc + argv);
    fwi::io::genericInputCardReader genericReader(arguments[0]);
    fwi::io::genericInput gInput = genericReader.getInput();

    std::string logFileName = gInput.outputLocation + gInput.runName + "Process.log";

    if(!gInput.verbose)
    {
        std::cout << "Printing the program output onto a file named: " << logFileName << " in the output folder" << std::endl;
        fwi::io::initLogger(logFileName.c_str(), fwi::io::ldebug);
    }

    fwi::io::chi_visualisation_in_integer_form(gInput.inputFolder + gInput.fileName + ".txt", gInput.nGridOriginal[0]);
    fwi::io::createCsvFilesForChi(gInput.inputFolder + gInput.fileName + ".txt", gInput, "chi_reference_");

    fwi::performance::CpuClock clock;

    clock.Start();
    performInversion(gInput, gInput.runName);
    clock.End();

    L_(fwi::io::linfo) << "Visualisation of the estimated temple using FWI";
    fwi::io::chi_visualisation_in_integer_form(gInput.outputLocation + "chi_est_" + gInput.runName + ".txt", gInput.nGrid[0]);
    fwi::io::createCsvFilesForChi(gInput.outputLocation + "chi_est_" + gInput.runName + ".txt", gInput, "chi_est_");

    std::string msg = clock.OutputString();
    writePlotInput(gInput, msg);
    fwi::io::endLogger();
    return 0;
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

void performInversion(const fwi::io::genericInput &gInput, const std::string &runName)
{
    // initialize the grid sources receivers, grouped frequencies
    fwi::core::grid2D grid(gInput.reservoirTopLeftCornerInM, gInput.reservoirBottomRightCornerInM, gInput.nGrid);
    fwi::core::sources src(gInput.sourcesTopLeftCornerInM, gInput.sourcesBottomRightCornerInM, gInput.nSources);
    src.Print();
    fwi::core::receivers recv(gInput.receiversTopLeftCornerInM, gInput.receiversBottomRightCornerInM, gInput.nReceivers);
    recv.Print();
    fwi::core::frequenciesGroup freq(gInput.freq, gInput.c0);
    freq.Print(gInput.freq.nTotal);

    int magnitude = freq.nFreq * src.nSrc * recv.nRecv;

    // read referencePressureData from a CSV file format
    std::vector<std::complex<double>> referencePressureData(magnitude);

    std::string fileLocation = gInput.outputLocation + runName + "InvertedChiToPressure.txt";
    std::ifstream file(fileLocation);
    fwi::io::CSVReader row;

    if(!file.is_open())
    {
        L_(fwi::io::linfo) << "Could not open file at " << fileLocation;
        exit(EXIT_FAILURE);
    }

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
    fwi::ForwardModelContainer forwardmodelcontainer(gInput, "integralForwardModel", grid, src, recv, freq);
    clock_t tEndForwardModel = clock();
    L_(fwi::io::linfo) << "Forwardmodel is created in " << double(tEndForwardModel - tStartForwardModel) / CLOCKS_PER_SEC << "seconds.";

    L_(fwi::io::linfo) << "Create inversionModel";
    clock_t tStartInversion = clock();
    fwi::inversionMethods::inversionInterface *inverse;
    inverse = new fwi::inversionMethods::OpenMPGradientDescentInversion(gInput, forwardmodelcontainer);
    clock_t tEndInversion = clock();
    L_(fwi::io::linfo) << "Inversionmodel is created in " << double(tEndInversion - tStartInversion) / CLOCKS_PER_SEC << "seconds.";

    L_(fwi::io::linfo) << "Estimating Chi...";
    clock_t tStartEstimateChi = clock();
    fwi::core::dataGrid2D chi_est = inverse->reconstruct(referencePressureData, gInput);
    clock_t tEndEstimateChi = clock();
    L_(fwi::io::linfo) << "Estimated Chi in " << double(tEndEstimateChi - tStartEstimateChi) / CLOCKS_PER_SEC << "seconds.";
    L_(fwi::io::linfo) << "Writing to file";

    chi_est.toFile(gInput.outputLocation + "chi_est_" + runName + ".txt");

    delete inverse;
}
