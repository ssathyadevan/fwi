#include "genericInputCardReader.h"
#include "utilityFunctions.h"
//#include "forwardModel.h"
#include "integralForwardModel.h"
#include "cpuClock.h"

void generateReferencePressureFieldFromChi(const genericInput &gInput, const std::string &runName);

int main(int argc, char **argv)
{
    std::vector<std::string> arguments = returnInputDirectory(argc, argv);
    genericInputCardReader genericReader(arguments[0]);
    const genericInput gInput = genericReader.getInput();

    //integralForwardModelInputCardReader forwardModelReader(gInput.caseFolder);
    //const integralForwardModelInput fmInput = forwardModelReader.getInput();

    if (!gInput.verbose)
    {
        WriteToFileNotToTerminal(gInput.outputLocation, gInput.runName, "PreProcess");
    }

    std::cout << "Preprocessing the provided input to create the reference pressure-field" << std::endl;

    cpuClock clock;

    clock.Start();
    generateReferencePressureFieldFromChi(gInput, gInput.runName);
    clock.End();
    clock.PrintTimeElapsed();

    return 0;
}

void generateReferencePressureFieldFromChi(const genericInput &gInput, const std::string &runName)
{
    // initialize the grid, sources, receivers, grouped frequencies
    grid2D grid(gInput.reservoirTopLeftCornerInM, gInput.reservoirBottomRightCornerInM, gInput.ngrid_original);
    pressureFieldSerial chi(grid);
    chi.fromFile(gInput);
    sources src(gInput.sourcesTopLeftCornerInM, gInput.sourcesBottomRightCornerInM, gInput.nSourcesReceivers.src);
    src.Print();
    receivers recv(gInput.receiversTopLeftCornerInM, gInput.receiversBottomRightCornerInM, gInput.nSourcesReceivers.rec);
    recv.Print();
    frequenciesGroup freqg(gInput.freq, gInput.c_0);
    freqg.Print(gInput.freq.nTotal);

    int magnitude = freqg.nFreq * src.nSrc * recv.nRecv;

    //std::complex<double>* referencePressureData = new std::complex<double>[magnitude];
    std::complex<double> referencePressureData[magnitude];

    chi.toFile(gInput.outputLocation + "chi_ref_" + runName + ".txt");

    ForwardModelInterface *model;
    model = new IntegralForwardModel(grid, src, recv, freqg, gInput);

    std::cout << "Calculate pData (the reference pressure-field)..." << std::endl;
    model->calculatePTot(chi);
    model->calculateKappa();
    model->calculatePData(chi, referencePressureData);

    // writing the referencePressureData to a text file in complex form
    std::cout << "calculateData done" << std::endl;

    std::string invertedChiToPressureFileName = gInput.outputLocation + runName + "InvertedChiToPressure.txt";
    std::ofstream file;
    file.open(invertedChiToPressureFileName, std::ios::out | std::ios::trunc);

    if (!file)
    {
        std::cout << "Failed to open the file to store inverted chi to pressure field" << std::endl;
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
