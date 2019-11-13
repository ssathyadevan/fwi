#include "conjugateGradientInversion.h"
#include "factory.h"
#include "inversionInterface.h"
#include "inputCardReader.h"
#include "genericInputCardReader.h"
#include "utilityFunctions.h"
#include "chiIntegerVisualisation.h"
#include "createChiCSV.h"
#include "csvReader.h"
#include "cpuClock.h"
#include <string>
#include <mpi.h>

void performInversion(const GenericInput &gInput, const std::string &runName, const std::string desired_inversion, const std::string desired_forward_model, const int mpi_rank);
void writePlotInput(const GenericInput &gInput);

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        std::cout << "Please give the case folder as argument. The case folder should contain an input and output folder." << std::endl;
        std::cout << "Make sure the input folder inside the case folder contains the files GenericInput.json, FMInput.json and CGInput.json" << std::endl;

        std::cout << std::endl
                  << "Please specify the desired inversion method" << std::endl;
        std::cout << "Make sure the inversion method has been added as indicated in how_to_add_an_inversion_method.pdf" << std::endl;

        std::cout << std::endl
                  << "Please specify the desired forward model" << std::endl;
        std::cout << "Make sure the forward model has been added as indicated in how_to_add_an_inversion_method.pdf" << std::endl;

        exit(EXIT_FAILURE);
    }
    int mpi_initialized, mpi_finalized, mpi_rank, mpi_size;
    MPI_Initialized(&mpi_initialized);
    if (!mpi_initialized) MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

    std::vector<std::string> arguments(argv + 1, argc + argv);
    GenericInputCardReader genericReader(arguments[0]);
    GenericInput gInput = genericReader.getInput();
    std::string desired_inversion = arguments[1];
    std::string desired_forward_model = arguments[2];
    time_t t1, t2;

    if (!gInput.verbose)
    {
        WriteToFileNotToTerminal(gInput.outputLocation, gInput.runName, "Process");
    }
    
    if (mpi_rank == 0) { //MASTER THREAD
        chi_visualisation_in_integer_form(gInput.inputFolder + gInput.fileName + ".txt", gInput.ngrid_original[0]);
        create_csv_files_for_chi(gInput.inputFolder + gInput.fileName + ".txt", gInput, "chi_reference_");

        

        t1 = (time_t) MPI_Wtime();
        std::cout << "Starting at " << std::asctime(std::localtime(&t1)) << std::endl;
    }

        performInversion(gInput, gInput.runName, desired_inversion, desired_forward_model, mpi_rank);

    if (mpi_rank == 0) { //MASTER THREAD
        t2 = (time_t) MPI_Wtime();
        std::cout << "Finished at " << std::asctime(std::localtime(&t2)) << std::endl;


        std::cout << "Visualisation of the estimated temple using FWI" << std::endl;
        chi_visualisation_in_integer_form(gInput.outputLocation + "chi_est_" + gInput.runName + ".txt", gInput.ngrid[0]);
        create_csv_files_for_chi(gInput.outputLocation + "chi_est_" + gInput.runName + ".txt", gInput, "chi_est_");

        writePlotInput(gInput);
    }


        MPI_Finalized(&mpi_finalized);
        if (!mpi_finalized) MPI_Finalize();

        return 0;
}

void writePlotInput(const GenericInput &gInput)
{
    // This part is needed for plotting the chi values in postProcessing.py
    std::ofstream outputfwi;
    std::string runName = gInput.runName;
    outputfwi.open(gInput.outputLocation + runName + ".pythonIn");
    outputfwi << "This run was parametrized as follows:" << std::endl;
    outputfwi << "nxt   = " << gInput.ngrid[0] << std::endl;
    outputfwi << "nzt   = " << gInput.ngrid[1] << std::endl;
    outputfwi << "nxt_original   = " << gInput.ngrid_original[0] << std::endl;
    outputfwi << "nzt_original   = " << gInput.ngrid_original[1] << std::endl;
    outputfwi.close();

    // This part is needed for plotting the chi values in postProcessing.py
    std::ofstream lastrun;
    lastrun.open(gInput.outputLocation + "/lastRunName.txt");
    lastrun << runName;
    lastrun.close();
}

void performInversion(const GenericInput &gInput, const std::string &runName, const std::string desired_inversion, const std::string desired_forward_model, const int mpi_rank)
{

    // initialize the grid, sources, receivers, grouped frequencies
    Grid2D grid(gInput.reservoirTopLeftCornerInM, gInput.reservoirBottomRightCornerInM, gInput.ngrid);
    Sources src(gInput.sourcesTopLeftCornerInM, gInput.sourcesBottomRightCornerInM, gInput.nSourcesReceivers.src);
    src.Print();
    Receivers recv(gInput.receiversTopLeftCornerInM, gInput.receiversBottomRightCornerInM, gInput.nSourcesReceivers.rec);
    recv.Print();
    FrequenciesGroup freq(gInput.freq, gInput.c_0);
    freq.Print(gInput.freq.nTotal);

    int magnitude = freq.nFreq * src.nSrc * recv.nRecv;
    //read referencePressureData from a CSV file format
    std::vector<std::complex<double>> referencePressureData(magnitude);

    if (mpi_rank==0){ //MASTER
        std::string fileLocation = gInput.outputLocation + runName + "InvertedChiToPressure.txt";
        std::ifstream file(fileLocation);
        CSVReader row;

        if (!file.is_open())
        {
            std::cout << "Could not open file at " << fileLocation;
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

    ConjugateGradientInversion *inverse;
    inverse = new ConjugateGradientInversion(model, gInput);

    if (mpi_rank==0){
         std::cout << "Estimating Chi..." << std::endl;

        PressureFieldSerial chi_est = inverse->ReconstructMPI(referencePressureData, gInput);

        std::cout << "Done, writing to file" << std::endl;

        chi_est.toFile(gInput.outputLocation + "chi_est_" + runName + ".txt");
    }else{
        inverse->ReconstructMPISlave(gInput);
    }

    delete model;
    delete inverse;
}


