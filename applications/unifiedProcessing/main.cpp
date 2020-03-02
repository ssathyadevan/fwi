#include "factory.h"
#include "inversionInterface.h"
#include "inputCardReader.h"
#include "genericInputCardReader.h"
#include "utilityFunctions.h"
#include "chiIntegerVisualisation.h"
#include "createChiCSV.h"
#include "csvReader.h"
#include "cpuClock.h"
#include "log.h"
#include <string>

void performInversion(const GenericInput &gInput, const std::string &runName, const std::string desired_inversion, const std::string desired_forward_model);
void writePlotInput(const GenericInput &gInput, std::string msg);

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        L_(linfo) << "Please give the case folder as argument. The case folder should contain an input and output folder." << std::endl;
        L_(linfo) << "Make sure the input folder inside the case folder contains the files GenericInput.json, FMInput.json and CGInput.json" << std::endl;

        L_(linfo) << std::endl
                  << "Please specify the desired inversion method" << std::endl;
        L_(linfo) << "Make sure the inversion method has been added as indicated in how_to_add_an_inversion_method.pdf" << std::endl;

        L_(linfo) << std::endl
                  << "Please specify the desired forward model" << std::endl;
        L_(linfo) << "Make sure the forward model has been added as indicated in how_to_add_an_inversion_method.pdf" << std::endl;

        exit(EXIT_FAILURE);
    }

    try{
        std::vector<std::string> arguments(argv + 1, argc + argv);
        GenericInputCardReader genericReader(arguments[0]);
        GenericInput gInput = genericReader.getInput();
        std::string desired_inversion = arguments[1];
        std::string desired_forward_model = arguments[2];

        std::string logFileName =  gInput.outputLocation + gInput.runName + "Process.log";

        if (!gInput.verbose)
        {
            std::cout << "Printing the program output onto a file named: " << logFileName << " in the output folder" << std::endl;
            initLogger( logFileName.c_str(), ldebug);
        }


        L_(linfo) << "Test";

        chi_visualisation_in_integer_form(gInput.inputFolder + gInput.fileName + ".txt", gInput.ngrid_original[0]);
        create_csv_files_for_chi(gInput.inputFolder + gInput.fileName + ".txt", gInput, "chi_reference_");

        CpuClock clock;

        clock.Start();
        performInversion(gInput, gInput.runName, desired_inversion, desired_forward_model);
        clock.End();

        L_(linfo) << "Visualisation of the estimated temple using FWI" ;
        chi_visualisation_in_integer_form(gInput.outputLocation + "chi_est_" + gInput.runName + ".txt", gInput.ngrid[0]);
        create_csv_files_for_chi(gInput.outputLocation + "chi_est_" + gInput.runName + ".txt", gInput, "chi_est_");

        std::string msg = clock.OutputString();
        writePlotInput(gInput, msg);
        endLogger();
    }
    catch(const std::invalid_argument& e){
        std::cout << "An invalid argument found!" << std::endl;
        std::cout<< e.what() << std::endl;
    }
    catch( const std::exception& e){
        std::cout<< e.what()<< std::endl;
    }

    return 0;
}

void writePlotInput(const GenericInput &gInput, std::string msg)
{
    // This part is needed for plotting the chi values in postProcessing.py
    std::ofstream outputfwi;
    std::string runName = gInput.runName;
    outputfwi.open(gInput.outputLocation + runName + ".pythonIn");
    outputfwi << "This run was parametrized as follows:" << std::endl;
    outputfwi << "nxt   = " << gInput.ngrid[0] << std::endl;
    outputfwi << "nzt   = " << gInput.ngrid[1] << std::endl;
    outputfwi << "nxt_original   = " << gInput.ngrid_original[0] << std::endl;
    outputfwi << "nzt_original   = " << gInput.ngrid_original[1] << std::endl << msg;
    outputfwi.close();

    // This part is needed for plotting the chi values in postProcessing.py
    std::ofstream lastrun;
    lastrun.open(gInput.outputLocation + "/lastRunName.txt");
    lastrun << runName;
    lastrun.close();
}

void performInversion(const GenericInput &gInput, const std::string &runName, const std::string desired_inversion, const std::string desired_forward_model)
{

    // initialize the grid, sources, receivers, grouped frequencies
    Grid2D grid(gInput.reservoirTopLeftCornerInM, gInput.reservoirBottomRightCornerInM, gInput.ngrid);
    Sources src(gInput.sourcesTopLeftCornerInM, gInput.sourcesBottomRightCornerInM, gInput.nSourcesReceivers.nsources);
    src.Print();
    Receivers recv(gInput.receiversTopLeftCornerInM, gInput.receiversBottomRightCornerInM, gInput.nSourcesReceivers.nreceivers);
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
        L_(linfo) << "Could not open file at " << fileLocation;
        exit(EXIT_FAILURE);
    }

    int i = 0;
    L_(linfo) << "Read reference data" << fileLocation;
    while (file >> row)
    {
        if (i < magnitude)
        {
            referencePressureData[i] = {atof(row[0].c_str()), atof(row[1].c_str())};
        }
        i++;
    }
    L_(linfo) << "Create ForwardModel";
    ForwardModelInterface *model;
    model = Factory::createForwardModel(gInput, desired_forward_model, grid, src, recv, freq);

    L_(linfo) << "Create InversionModel";
    InversionInterface *inverse;
    inverse = Factory::createInversion(desired_inversion, model, gInput);

    L_(linfo) << "Estimating Chi..." ;

    PressureFieldSerial chi_est = inverse->Reconstruct(referencePressureData, gInput);

    L_(linfo) << "Done, writing to file" ;

    chi_est.toFile(gInput.outputLocation + "chi_est_" + runName + ".txt");

    delete model;
    delete inverse;
}


