#include "genericInputCardReader.h"
#include "utilityFunctions.h"
//#include "forwardModel.h"
#include "cpuClock.h"
#include "finiteDifferenceForwardModel.h"
#include "finiteDifferenceForwardModelInputCardReader.h"
#include "log.h"

void generateReferencePressureFieldFromChi(const genericInput &gInput, const std::string &runName);

int main(int argc, char **argv)
{
    try
    {
        std::vector<std::string> arguments = returnInputDirectory(argc, argv);
        genericInputCardReader genericReader(arguments[0]);
        const genericInput gInput = genericReader.getInput();

        std::string logFileName = gInput.outputLocation + gInput.runName + "PreProcess.log";

        if(!gInput.verbose)
        {
            std::cout << "Printing the program output onto a file named: " << logFileName << " in the output folder" << std::endl;
            initLogger(logFileName.c_str(), ldebug);
        }

        L_(linfo) << "Preprocessing the provided input to create the reference pressure-field";

        CpuClock clock;

        clock.Start();
        generateReferencePressureFieldFromChi(gInput, gInput.runName);
        clock.End();
    }
    catch(const std::invalid_argument &e)
    {
        std::cout << "An invalid argument found!" << std::endl;
        std::cout << e.what() << std::endl;
        L_(linfo) << "Invalid Argument Exception: " << e.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }
    catch(const std::exception &e)
    {
        std::cout << "An exception has been thrown:" << std::endl;
        std::cout << e.what() << std::endl;
        L_(linfo) << "Exception: " << e.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }

    return 0;
}

void generateReferencePressureFieldFromChi(const genericInput &gInput, const std::string &runName)
{
    // initialize the grid sources receivers, grouped frequencies
    core::grid2D grid(gInput.reservoirTopLeftCornerInM, gInput.reservoirBottomRightCornerInM, gInput.nGridOriginal);
    core::dataGrid2D chi(grid);

    std::string inputPath = gInput.inputFolder + gInput.fileName + ".txt";
    chi.fromFile(inputPath);

    core::sources src(gInput.sourcesTopLeftCornerInM, gInput.sourcesBottomRightCornerInM, gInput.nSources);
    src.Print();

    core::receivers recv(gInput.receiversTopLeftCornerInM, gInput.receiversBottomRightCornerInM, gInput.nReceivers);
    recv.Print();

    core::frequenciesGroup freqg(gInput.freq, gInput.c0);
    freqg.Print(gInput.freq.nTotal);

    int magnitude = freqg.nFreq * src.nSrc * recv.nRecv;
    std::vector<std::complex<double>> referencePressureData(magnitude);

    std::string outputPath = gInput.outputLocation + "chi_ref_" + runName + ".txt";
    chi.toFile(outputPath);

    L_(linfo) << "Create forwardModel";
    clock_t tStartForwardModel = clock();
    forwardModelInterface *model;
    finiteDifferenceForwardModelInputCardReader finiteDifferenceReader(gInput.caseFolder);
    finiteDifferenceForwardModelInput fmInput = finiteDifferenceReader.getInput();
    model = new finiteDifferenceForwardModel(grid, src, recv, freqg, fmInput);
    clock_t tEndForwardModel = clock();
    L_(linfo) << "Forwardmodel is created in " << double(tEndForwardModel - tStartForwardModel) / CLOCKS_PER_SEC << "seconds.";

    L_(linfo) << "Calculating pData (the reference pressure-field)...";
    clock_t tStartCalculatePData = clock();
    model->calculatePTot(chi);
    model->calculateKappa();
    model->calculatePData(chi, referencePressureData);
    clock_t tEndCalculatePData = clock();
    L_(linfo) << "PData has been calculated in " << double(tEndCalculatePData - tStartCalculatePData) / CLOCKS_PER_SEC << "seconds.";
    // writing the referencePressureData to a text file in complex form
    L_(linfo) << "calculateData done";

    std::string invertedChiToPressureFileName = gInput.outputLocation + runName + "InvertedChiToPressure.txt";
    std::ofstream file;
    file.open(invertedChiToPressureFileName, std::ios::out | std::ios::trunc);

    if(!file)
    {
        L_(lerror) << "Failed to open the file to store inverted chi to pressure field" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    for(int i = 0; i < magnitude; i++)
    {
        file << std::setprecision(17) << referencePressureData[i].real() << "," << referencePressureData[i].imag() << std::endl;
    }

    file.close();

    delete model;
}
