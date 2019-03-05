#include "forwardModelInputCardReader.h"
#include "genericInputCardReader.h"
#include "utilityFunctions.h"
//#include "forwardModel.h"
#include "forwardModelBasicOptimization.h"
#include "cpuClock.h"


void generateReferencePressureFieldFromChi(const genericInput& gInput, const forwardModelInput& fmInput, const std::string& runName);

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::cout << "Please enter 2 arguments. 1st the location of the input card set, 2nd the output folder location," << std::endl;
        std::cout << "Make sure your card set contains the GenericInput.in and ForwardModelInput.in files" << std::endl;
        std::cout << "e.g. ../input/default/ ../output/" << std::endl;

        exit(EXIT_FAILURE);
    }

    std::vector<std::string> arguments(argv+1, argc+argv);
    std::string inputFolder  = arguments[0];
    std::string outputFolder = arguments[1];

    genericInputCardReader genericReader(inputFolder, outputFolder);
    forwardModelInputCardReader forwardModelReader(inputFolder);
    const genericInput      gInput  = genericReader.getInput();
    const forwardModelInput fmInput = forwardModelReader.getInput();

    if (!gInput.verbose)
    {
        WriteToFileNotToTerminal(gInput.outputLocation, gInput.runName, "PreProcess");
    }

    std::cout << "Preprocessing the provided input to create the reference pressure-field" << std::endl;

    cpuClock clock;

    clock.Start();
    generateReferencePressureFieldFromChi(gInput, fmInput, gInput.runName);
    clock.End();
    clock.PrintTimeElapsed();

    return 0;
}

void generateReferencePressureFieldFromChi (const genericInput& gInput, const forwardModelInput& fmInput, const std::string& runName)
{
    // initialize the grid, sources, receivers, grouped frequencies
    grid2D grid(gInput.reservoirTopLeftCornerInM, gInput.reservoirBottomRightCornerInM, gInput.ngrid);
    pressureFieldSerial chi(grid);
    chi.fromFile(gInput);
    sources src(gInput.sourcesTopLeftCornerInM, gInput.sourcesBottomRightCornerInM, gInput.nSourcesReceivers.src);
    src.Print();
    receivers recv(gInput.receiversTopLeftCornerInM, gInput.receiversBottomRightCornerInM, gInput.nSourcesReceivers.rec);
    recv.Print();
    frequenciesGroup freqg(gInput.freq, gInput.c_0);
    freqg.Print(gInput.freq.nTotal);

    int magnitude = gInput.freq.nTotal * gInput.nSourcesReceivers.src * gInput.nSourcesReceivers.rec;

    std::complex<double> referencePressureData[magnitude];

    chi.toFile(gInput.outputLocation + "chi_ref_" + runName + ".txt");

    ForwardModelInterface *model;
    //model = new forwardModel(grid, src, recv, freqg, fmInput);
    model = new forwardModelBasicOptimization(grid, src, recv, freqg, fmInput);


    std::cout << "Calculate pData (the reference pressure-field)..." << std::endl;
    model->createPdataEst(referencePressureData, chi);

    // writing the referencePressureData to a text file in complex form
    std::cout << "calculateData done" << std::endl;

    std::string invertedChiToPressureFileName = gInput.outputLocation + runName + "InvertedChiToPressure.txt";
    std::ofstream file;
    file.open (invertedChiToPressureFileName, std::ios::out | std::ios::trunc);

    if (!file)
    {
        std::cout<< "Failed to open the file to store inverted chi to pressure field" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    for(int i=0; i < magnitude; i++)
    {
        file << std::setprecision(17) << referencePressureData[i].real()
             <<"," << referencePressureData[i].imag() << std::endl;
    }

    file.close();

    delete model;

}


