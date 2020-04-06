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
    }
    catch(const std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}

void generateReferencePressureFieldFromChi(const genericInput &gInput, const std::string &runName)
{
    // initialize the grid, sources, receivers, grouped frequencies
    grid2D grid(gInput.reservoirTopLeftCornerInM, gInput.reservoirBottomRightCornerInM, gInput.nGridOriginal);
    dataGrid2D chi(grid);

    chi.fromFile(gInput);
    sources src(gInput.sourcesTopLeftCornerInM, gInput.sourcesBottomRightCornerInM, gInput.nSources);
    src.Print();
    receivers recv(gInput.receiversTopLeftCornerInM, gInput.receiversBottomRightCornerInM, gInput.nReceivers);
    recv.Print();
    frequenciesGroup freqg(gInput.freq, gInput.c0);
    freqg.Print(gInput.freq.nTotal);

    int magnitude = freqg.nFreq * src.nSrc * recv.nRecv;

    // std::complex<double>* referencePressureData = new std::complex<double>[magnitude];
    std::vector<std::complex<double>> referencePressureData(magnitude);

    chi.toFile(gInput.outputLocation + "chi_ref_" + runName + ".txt");

    forwardModelInterface *model;
    finiteDifferenceForwardModelInputCardReader finiteDifferenceReader(gInput.caseFolder);
    finiteDifferenceForwardModelInput fmInput = finiteDifferenceReader.getInput();
    model = new finiteDifferenceForwardModel(grid, src, recv, freqg, fmInput);

    L_(linfo) << "Calculate pData (the reference pressure-field)...";
    model->calculatePTot(chi);
    model->calculateKappa();
    model->calculatePData(chi, referencePressureData);

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
