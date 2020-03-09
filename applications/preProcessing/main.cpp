#include "genericInputCardReader.h"
#include "utilityFunctions.h"
//#include "forwardModel.h"
#include "integralForwardModel.h"
#include "integralForwardModelInputCardReader.h"
#include "cpuClock.h"
#include "log.h"

void generateReferencePressureFieldFromChi(const GenericInput &gInput, const std::string &runName);

int main(int argc, char **argv)
{
    std::vector<std::string> arguments = returnInputDirectory(argc, argv);
    GenericInputCardReader genericReader(arguments[0]);
    const GenericInput gInput = genericReader.getInput();

    std::string logFileName =  gInput.outputLocation + gInput.runName + "PreProcess.log";

    if (!gInput.verbose)
    {
        std::cout << "Printing the program output onto a file named: " << logFileName << " in the output folder" << std::endl;
        initLogger( logFileName.c_str(), ldebug);
    }

    L_(linfo) << "Preprocessing the provided input to create the reference pressure-field" ;

    CpuClock clock;

    clock.Start();
    generateReferencePressureFieldFromChi(gInput, gInput.runName);
    clock.End();

    return 0;
}

void generateReferencePressureFieldFromChi(const GenericInput &gInput, const std::string &runName)
{
    // initialize the grid, Sources, receivers, grouped frequencies
    Grid2D grid(gInput.reservoirTopLeftCornerInM, gInput.reservoirBottomRightCornerInM, gInput.ngrid_original);
    PressureFieldSerial chi(grid);
    chi.fromFile(gInput);
    Sources src(gInput.sourcesTopLeftCornerInM, gInput.sourcesBottomRightCornerInM, gInput.nSourcesReceivers.nsources);
    src.Print();
    Receivers recv(gInput.receiversTopLeftCornerInM, gInput.receiversBottomRightCornerInM, gInput.nSourcesReceivers.nreceivers);
    recv.Print();
    FrequenciesGroup freqg(gInput.freq, gInput.c_0);
    freqg.Print(gInput.freq.nTotal);

    int magnitude = freqg.nFreq * src.nSrc * recv.nRecv;

    //std::complex<double>* referencePressureData = new std::complex<double>[magnitude];
    std::vector<std::complex<double>> referencePressureData(magnitude);

    chi.toFile(gInput.outputLocation + "chi_ref_" + runName + ".txt");

    ForwardModelInterface *model;
    IntegralForwardModelInputCardReader integralreader(gInput.caseFolder);
    model = new IntegralForwardModel(grid, src, recv, freqg, integralreader.getInput());

    L_(linfo) << "Calculate pData (the reference pressure-field)..." ;
    model->calculatePTot(chi);
    model->calculateKappa();
    model->calculatePData(chi, referencePressureData);

    // writing the referencePressureData to a text file in complex form
    L_(linfo) << "calculateData done" ;

    std::string invertedChiToPressureFileName = gInput.outputLocation + runName + "InvertedChiToPressure.txt";
    std::ofstream file;
    file.open(invertedChiToPressureFileName, std::ios::out | std::ios::trunc);

    if (!file)
    {
        L_(lerror) << "Failed to open the file to store inverted chi to pressure field" ;
        std::exit(EXIT_FAILURE);
    }

    for (int i = 0; i < magnitude; i++)
    {
        file << std::setprecision(17) << referencePressureData[i].real()
             << "," << referencePressureData[i].imag() << std::endl;
    }

    file.close();

    delete model;
}
