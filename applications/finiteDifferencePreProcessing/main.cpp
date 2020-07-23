#include "FiniteDifferenceForwardModel.h"
#include "FiniteDifferenceForwardModelInputCardReader.h"
#include "cpuClock.h"
#include "genericInputCardReader.h"
#include "log.h"
#include "utilityFunctions.h"

void generateReferencePressureFieldFromChi(const fwi::io::genericInput &gInput, const std::string &runName);

int main(int argc, char **argv)
{
    try
    {
        std::vector<std::string> arguments = fwi::utilities::returnInputDirectory(argc, argv);
        fwi::io::genericInputCardReader genericReader(arguments[0]);
        const fwi::io::genericInput gInput = genericReader.getInput();

        std::string logFileName = gInput.outputLocation + gInput.runName + "PreProcess.log";

        if(!gInput.verbose)
        {
            std::cout << "Printing the program output onto a file named: " << logFileName << " in the output folder" << std::endl;
            fwi::io::initLogger(logFileName.c_str(), fwi::io::ldebug);
        }

        L_(fwi::io::linfo) << "Preprocessing the provided input to create the reference pressure-field";

        fwi::performance::CpuClock clock;

        clock.Start();
        generateReferencePressureFieldFromChi(gInput, gInput.runName);
        clock.End();
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

void generateReferencePressureFieldFromChi(const fwi::io::genericInput &gInput, const std::string &runName)
{
    // initialize the grid sources receivers, grouped frequencies
    fwi::core::grid2D grid(gInput.reservoirTopLeftCornerInM, gInput.reservoirBottomRightCornerInM, gInput.nGridOriginal);
    fwi::core::dataGrid2D chi(grid);

    std::string inputPath = gInput.inputFolder + gInput.fileName + ".txt";
    chi.fromFile(inputPath);

    fwi::core::Sources source(gInput.sourcesTopLeftCornerInM, gInput.sourcesBottomRightCornerInM, gInput.nSources);
    source.Print();

    fwi::core::Receivers receiver(gInput.receiversTopLeftCornerInM, gInput.receiversBottomRightCornerInM, gInput.nReceivers);
    receiver.Print();

    fwi::core::FrequenciesGroup freqg(gInput.freq, gInput.c0);
    freqg.Print(gInput.freq.nTotal);

    int magnitude = freqg.count * source.count * receiver.count;
    std::vector<std::complex<double>> referencePressureData(magnitude);

    std::string outputPath = gInput.outputLocation + "chi_ref_" + runName + ".txt";
    chi.toFile(outputPath);

    L_(fwi::io::linfo) << "Create forwardModel";
    clock_t tStartForwardModel = clock();
    fwi::forwardModels::forwardModelInterface *model;
    fwi::forwardModels::finiteDifferenceForwardModelInputCardReader finiteDifferenceReader(gInput.caseFolder);
    fwi::forwardModels::finiteDifferenceForwardModelInput fmInput = finiteDifferenceReader.getInput();
    model = new fwi::forwardModels::finiteDifferenceForwardModel(grid, source, receiver, freqg, fmInput);
    clock_t tEndForwardModel = clock();
    L_(fwi::io::linfo) << "Forwardmodel is created in " << double(tEndForwardModel - tStartForwardModel) / CLOCKS_PER_SEC << "seconds.";

    L_(fwi::io::linfo) << "Calculating pData (the reference pressure-field)...";
    clock_t tStartCalculatePData = clock();
    model->calculatePTot(chi);
    model->calculateKappa();
    model->calculatePData(chi, referencePressureData);
    clock_t tEndCalculatePData = clock();
    L_(fwi::io::linfo) << "PData has been calculated in " << double(tEndCalculatePData - tStartCalculatePData) / CLOCKS_PER_SEC << "seconds.";
    // writing the referencePressureData to a text file in complex form
    L_(fwi::io::linfo) << "calculateData done";

    std::string invertedChiToPressureFileName = gInput.outputLocation + runName + "InvertedChiToPressure.txt";
    std::ofstream file;
    file.open(invertedChiToPressureFileName, std::ios::out | std::ios::trunc);

    if(!file)
    {
        L_(fwi::io::lerror) << "Failed to open the file to store inverted chi to pressure field" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    for(int i = 0; i < magnitude; i++)
    {
        file << std::setprecision(17) << referencePressureData[i].real() << "," << referencePressureData[i].imag() << std::endl;
    }

    file.close();

    delete model;
}
