#include "genericInputCardReader.h"
#include "utilityFunctions.h"
//#include "forwardModel.h"
#include "cpuClock.h"
#include "integralForwardModel.h"
#include "integralForwardModelInputCardReader.h"
#include "log.h"

void generateReferencePressureFieldFromChi(const io::genericInput &gInput, const std::string &runName);

int main(int argc, char **argv)
{
    try
    {
        std::vector<std::string> arguments = returnInputDirectory(argc, argv);
        io::genericInputCardReader genericReader(arguments[0]);
        const io::genericInput gInput = genericReader.getInput();

        std::string logFilePath = gInput.outputLocation + gInput.runName + "PreProcess.log";

        if(!gInput.verbose)
        {
            std::cout << "Printing the program output into" << logFilePath << std::endl;
            initLogger(logFilePath.c_str(), io::ldebug);
        }

        L_(io::linfo) << "Preprocessing the provided input to create the reference pressure-field";

        CpuClock clock;
        clock.Start();
        generateReferencePressureFieldFromChi(gInput, gInput.runName);
        clock.End();
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

void generateReferencePressureFieldFromChi(const io::genericInput &gInput, const std::string &runName)
{
    // initialize the grid sources receivers, grouped frequencies
    core::grid2D grid(gInput.reservoirTopLeftCornerInM, gInput.reservoirBottomRightCornerInM, gInput.nGridOriginal);

    std::string inputFolder = gInput.inputFolder + gInput.fileName + ".txt";
    core::dataGrid2D chi(grid);
    chi.fromFile(inputFolder);

    core::sources src(gInput.sourcesTopLeftCornerInM, gInput.sourcesBottomRightCornerInM, gInput.nSources);
    src.Print();

    core::receivers recv(gInput.receiversTopLeftCornerInM, gInput.receiversBottomRightCornerInM, gInput.nReceivers);
    recv.Print();

    core::frequenciesGroup freqg(gInput.freq, gInput.c0);
    freqg.Print(gInput.freq.nTotal);

    int magnitude = freqg.nFreq * src.nSrc * recv.nRecv;

    // std::complex<double>* referencePressureData = new std::complex<double>[magnitude];
    std::vector<std::complex<double>> referencePressureData(magnitude);

    chi.toFile(gInput.outputLocation + "chi_ref_" + runName + ".txt");

    forwardModels::forwardModelInterface *model;
    forwardModels::integralForwardModelInputCardReader integralreader(gInput.caseFolder);
    model = new forwardModels::IntegralForwardModel(grid, src, recv, freqg, integralreader.getInput());

    L_(io::linfo) << "Calculate pData (the reference pressure-field)...";
    model->calculatePTot(chi);
    model->calculateKappa();
    model->calculatePData(chi, referencePressureData);

    // writing the referencePressureData to a text file in complex form
    L_(io::linfo) << "calculateData done";

    std::string invertedChiToPressureFileName = gInput.outputLocation + runName + "InvertedChiToPressure.txt";
    std::ofstream file;
    file.open(invertedChiToPressureFileName, std::ios::out | std::ios::trunc);

    if(!file)
    {
        L_(io::lerror) << "Failed to open the file to store inverted chi to pressure field";
        std::exit(EXIT_FAILURE);
    }

    for(int i = 0; i < magnitude; i++)
    {
        file << std::setprecision(17) << referencePressureData[i].real() << "," << referencePressureData[i].imag() << std::endl;
    }

    file.close();

    delete model;
}
