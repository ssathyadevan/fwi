#include "MPIConjugateGradientInversion.h"
#include "MPIConjugateGradientInversionInputCardReader.h"
#include "factory.h"
#include "integralForwardModel.h"
#include "inversionInterface.h"
#include "inputCardReader.h"
#include "genericInputCardReader.h"
#include "utilityFunctions.h"
#include "chiIntegerVisualisation.h"
#include "createChiCSV.h"
#include "csvReader.h"
#include "cpuClockMPI.h"
#include <string>
#include <mpi.h>
#include "log.h"

void performInversion(const GenericInput &gInput, const std::string &runName, const int mpi_rank, std::string desired_forward_model);
void writePlotInput(const GenericInput &gInput, std::string msg);

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        L_(lerror) << "Please give the case folder as argument. The case folder should contain an input and output folder." << std::endl;
        L_(lerror) << "Make sure the input folder inside the case folder contains the files GenericInput.json, FMInput.json and CGInput.json" << std::endl;

        L_(lerror) << "Please give the forward model as argument." << std::endl;
        exit(EXIT_FAILURE);
    }
    try{
        CpuClockMPI clock;
        int mpi_initialized, mpi_finalized, mpi_rank, mpi_size;
        MPI_Initialized(&mpi_initialized);
        if (!mpi_initialized)
            MPI_Init(&argc, &argv);

        MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
        MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
        std::vector<std::string> arguments(argv + 1, argc + argv);
        GenericInputCardReader genericReader(arguments[0]);
        GenericInput gInput = genericReader.getInput();
        std::string desired_forward_model = arguments[1];

        std::string logFileName;

        logFileName =  gInput.outputLocation + gInput.runName + "Process" + std::to_string(mpi_rank) + ".log";

        if (!gInput.verbose)
        {
            std::cout << "Printing the program output onto a file named: " << logFileName << " in the output folder" << std::endl;
            initLogger( logFileName.c_str(), ldebug);
        }

        if (mpi_rank == 0)
        { //MASTER THREAD
            std::cerr << "MPI initialized with " << mpi_size << " threads." << std::endl;
            chi_visualisation_in_integer_form(gInput.inputFolder + gInput.fileName + ".txt", gInput.ngrid_original[0]);
            create_csv_files_for_chi(gInput.inputFolder + gInput.fileName + ".txt", gInput, "chi_reference_");
            clock.Start();
        }

        performInversion(gInput, gInput.runName, mpi_rank, desired_forward_model);

        if (mpi_rank == 0)
        { //MASTER THREAD
            clock.End();

            L_(linfo) << "Visualisation of the estimated temple using FWI" ;
            chi_visualisation_in_integer_form(gInput.outputLocation + "chi_est_" + gInput.runName + ".txt", gInput.ngrid[0]);
            create_csv_files_for_chi(gInput.outputLocation + "chi_est_" + gInput.runName + ".txt", gInput, "chi_est_");

            std::string msg = clock.OutputString();
            writePlotInput(gInput, msg);

        }

        MPI_Finalized(&mpi_finalized);
        if (!mpi_finalized)
            MPI_Finalize();
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

void performInversion(const GenericInput &gInput, const std::string &runName, const int mpi_rank, std::string desired_forward_model)
{
    //CpuClockMPI a;

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
    if (mpi_rank == 0)
    { //MASTER
        std::string fileLocation = gInput.outputLocation + runName + "InvertedChiToPressure.txt";
        std::ifstream file(fileLocation);
        CSVReader row;

        if (!file.is_open())
        {
            L_(lerror) << "Could not open file at " << fileLocation;
            exit(EXIT_FAILURE);
        }

        int i = 0;
        while (file >> row)
        {
            if (i < magnitude)
            {
                referencePressureData[i] = {atof(row[0].c_str()), atof(row[1].c_str())};
            }
            i++;
        }
    }

    ForwardModelInterface *model;
    model = Factory::createForwardModel(gInput, desired_forward_model, grid, src, recv, freq);
    MPIConjugateGradientInversion *inverse;
    MPIConjugateGradientInversionInputCardReader mpiconjugategradientreader(gInput.caseFolder);
    inverse = new MPIConjugateGradientInversion(model, mpiconjugategradientreader.getInput());

    if (mpi_rank == 0)
    {
        L_(linfo) << "Estimating Chi..." ;

        PressureFieldSerial chi_est = inverse->Reconstruct(referencePressureData, gInput);

        L_(linfo) << "Done, writing to file" ;

        chi_est.toFile(gInput.outputLocation + "chi_est_" + runName + ".txt");
    }
    else
    {
        inverse->ReconstructSlave();
    }
    delete model;
    delete inverse;
}
