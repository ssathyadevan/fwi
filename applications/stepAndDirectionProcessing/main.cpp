#include "DirectionCalculator.h"
#include "StepAndDirectionReconstructor.h"
#include "StepAndDirectionReconstructorInputCardReader.h"
#include "StepSizeCalculator.h"
#include "chiIntegerVisualisation.h"
#include "cpuClock.h"
#include "createChiCSV.h"
#include "csvReader.h"
#include "factory.h"
#include "genericInputCardReader.h"
#include "inputCardReader.h"
#include "log.h"
#include "utilityFunctions.h"
#include <string>

void performInversion(const fwi::io::genericInput &gInput, const std::string &runName, const std::string desiredStepSize, const std::string desiredDirection,
    const std::string desired_forward_model);
void writePlotInput(const fwi::io::genericInput &gInput, std::string msg);

int main(int argc, char **argv)
{
    if(argc != 5)
    {
        L_(fwi::io::linfo) << "Please give the case folder as argument. The case folder should contain an input and output folder." << std::endl;
        L_(fwi::io::linfo) << "Make sure the input folder inside the case folder contains the files genericInput.json, FMInput.json and CGInput.json"
                           << std::endl;

        L_(fwi::io::linfo) << std::endl << "Please specify the desired inversion method" << std::endl;
        L_(fwi::io::linfo) << "Make sure the inversion method has been added as indicated in how_to_add_an_inversion_method.pdf" << std::endl;

        L_(fwi::io::linfo) << std::endl << "Please specify the desired forward model" << std::endl;
        L_(fwi::io::linfo) << "Make sure the forward model has been added as indicated in how_to_add_an_inversion_method.pdf" << std::endl;

        exit(EXIT_FAILURE);
    }

    try
    {
        std::vector<std::string> arguments(argv + 1, argc + argv);
        fwi::io::genericInputCardReader genericReader(arguments[0]);
        fwi::io::genericInput gInput = genericReader.getInput();
        std::string desiredStepSize = arguments[1];
        std::string desiredDirection = arguments[2];
        std::string desiredForwardModel = arguments[3];

        std::string logFileName = gInput.outputLocation + gInput.runName + "Process.log";

        if(!gInput.verbose)
        {
            std::cout << "Printing the program output onto a file named: " << logFileName << " in the output folder" << std::endl;
            initLogger(logFileName.c_str(), fwi::io::ldebug);
        }

        L_(fwi::io::linfo) << "Test";

        fwi::io::chi_visualisation_in_integer_form(gInput.inputFolder + gInput.fileName + ".txt", gInput.nGridOriginal[0]);
        createCsvFilesForChi(gInput.inputFolder + gInput.fileName + ".txt", gInput, "chi_reference_");

        fwi::performance::CpuClock clock;

        clock.Start();
        performInversion(gInput, gInput.runName, desiredStepSize, desiredDirection, desiredForwardModel);
        clock.End();

        L_(fwi::io::linfo) << "Visualisation of the estimated temple using FWI";
        fwi::io::chi_visualisation_in_integer_form(gInput.outputLocation + "chi_est_" + gInput.runName + ".txt", gInput.nGrid[0]);
        createCsvFilesForChi(gInput.outputLocation + "chi_est_" + gInput.runName + ".txt", gInput, "chi_est_");

        std::string msg = clock.OutputString();
        writePlotInput(gInput, msg);
        fwi::io::endLogger();
    }
    catch(const std::invalid_argument &e)
    {
        std::cout << "An invalid argument found!" << std::endl;
        std::cout << e.what() << std::endl;
    }
    catch(const std::exception &e)
    {
        std::cout << "Another exception is found!" << std::endl;
        std::cout << e.what() << std::endl;
    }

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

void performInversion(const fwi::io::genericInput &gInput, const std::string &runName, const std::string desiredStepSize, const std::string desiredDirection,
    const std::string desiredForwardModel)
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
    L_(fwi::io::linfo) << "Read reference data" << fileLocation;
    while(file >> row)
    {
        if(i < magnitude)
        {
            referencePressureData[i] = {atof(row[0].c_str()), atof(row[1].c_str())};
        }
        ++i;
    }
    fwi::Factory factory;

    L_(fwi::io::linfo) << "Create ForwardModel";
    fwi::forwardModels::forwardModelInterface *model;
    model = factory.createForwardModel(gInput.caseFolder, desiredForwardModel, grid, src, recv, freq);

    L_(fwi::io::linfo) << "Create StepAndDirectionReconstructor";
    fwi::inversionMethods::StepAndDirectionReconstructorInputCardReader stepAndDirectionReader(gInput.caseFolder);
    fwi::inversionMethods::StepAndDirectionReconstructorInput stepAndDirectionInput = stepAndDirectionReader.getInput();
    fwi::inversionMethods::StepAndDirectionReconstructor *reconstructor;
    reconstructor = factory.createStepAndDirectionReconstructor(stepAndDirectionInput, model, desiredStepSize, desiredDirection, referencePressureData);

    L_(fwi::io::linfo) << "Estimating Chi...";
    fwi::core::dataGrid2D chiEstimate = reconstructor->reconstruct(referencePressureData, gInput);

    L_(fwi::io::linfo) << "Done, writing to file";

    chiEstimate.toFile(gInput.outputLocation + "chi_est_" + runName + ".txt");
}
