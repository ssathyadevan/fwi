#include "HelpTextPreProcessing.h"
#include "argumentReader.h"
#include "cpuClock.h"
#include "factory.h"
#include "genericInputCardReader.h"
#include <iostream>
#include <vector>

void printHelpOrVersion(fwi::io::argumentReader &fwiOpts);
void executeFullFWI(const fwi::io::argumentReader &fwiOpts);
void doPreprocess(const fwi::io::genericInput &gInput);

int main(int argc, char *argv[])
{
    try
    {
        std::vector<std::string> arguments = {argv + 1, argv + argc};
        fwi::io::argumentReader fwiOpts(arguments);
        printHelpOrVersion(fwiOpts);

        fwi::io::genericInputCardReader genericReader(fwiOpts);
        const fwi::io::genericInput gInput = genericReader.getInput();
        doPreprocess(gInput);
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        std::cout << "Use -h for help on parameter options and values." << std::endl;
        L_(fwi::io::lerror) << e.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

void printHelpOrVersion(fwi::io::argumentReader &fwiOpts)
{
    if(fwiOpts.help)
    {
        std::cout << HELP_TEXT_PREPROCESSING << std::endl;
        std::exit(EXIT_SUCCESS);
    }

    if(fwiOpts.version)
    {
        std::cout << VERSION_PREPROCESSING << std::endl;
        std::exit(EXIT_SUCCESS);
    }
}

void doPreprocess(const fwi::io::genericInput &gInput)
{
    std::cout << "Starting preprocessing with\n  forward =" + gInput.forward << std::endl;

    // initialize the clock, grid sources receivers, grouped frequencies
    fwi::performance::CpuClock clock;
    fwi::core::grid2D grid(gInput.reservoirTopLeftCornerInM, gInput.reservoirBottomRightCornerInM, gInput.nGridOriginal);
    fwi::core::Sources source(gInput.sourcesTopLeftCornerInM, gInput.sourcesBottomRightCornerInM, gInput.nSources);
    fwi::core::Receivers receiver(gInput.receiversTopLeftCornerInM, gInput.receiversBottomRightCornerInM, gInput.nReceivers);
    fwi::core::FrequenciesGroup freq(gInput.freq, gInput.c0);

    // Initialize logger
    std::string logFilePath = gInput.outputLocation + gInput.runName + "PreProcess.log";
    if(!gInput.verbose)
    {
        std::cout << "Printing the preprocess output into file: " << logFilePath << std::endl;
        fwi::io::initLogger(logFilePath.c_str(), fwi::io::ldebug);
    }

    // Logging the setup
    L_(fwi::io::linfo) << "Starting PreProcess";
    source.Print();
    receiver.Print();
    freq.Print(gInput.freq.nTotal);

    // Start preprocess
    clock.Start();

    // Create Model
    L_(fwi::io::linfo) << "Create forwardModel";
    fwi::Factory factory;
    fwi::forwardModels::ForwardModelInterface *model = factory.createForwardModel(
        gInput.caseFolder, gInput.forward + "ForwardModel", grid, source, receiver, freq);

    // Read chi from file and write to output file
    fwi::core::dataGrid2D chi(grid);
    std::string inputPath = gInput.inputFolder + gInput.fileName + ".txt";
    chi.fromFile(inputPath);
    std::string outputPath = gInput.outputLocation + "chi_ref_" + gInput.runName + ".txt";
    chi.toFile(outputPath);

    // Calculate pressure field data
    std::cout << "Calculating..." << std::endl;
    L_(fwi::io::linfo) << "Calculating pData (the reference pressure-field)...";
    clock.Start();
    //model->calculatePTot(chi);
    model->calculateKappa();
    std::vector<std::complex<double>> referencePressureData = model->calculatePressureField(chi);

    clock.End();
    clock.OutputString();
    L_(fwi::io::linfo) << "calculateData done";

    // writing the referencePressureData to a text file in complex form
    std::string invertedChiToPressureFileName = gInput.outputLocation + gInput.runName + "InvertedChiToPressure.txt";
    L_(fwi::io::linfo) << "Writing pData to file " << invertedChiToPressureFileName;
    std::ofstream file;
    file.open(invertedChiToPressureFileName, std::ios::out | std::ios::trunc);
    if(!file)
    {
        L_(fwi::io::lerror) << "Failed to open the file to store inverted chi to pressure field";
        throw std::runtime_error("Could not open file at " + invertedChiToPressureFileName);
    }

    int magnitude = freq.count * source.count * receiver.count;
    for(int i = 0; i < magnitude; i++)
    {
        file << std::setprecision(17) << referencePressureData[i].real() << "," << referencePressureData[i].imag() << std::endl;
    }
    file.close();
    L_(fwi::io::linfo) << "PreProcessing Task finished successfully!";
    fwi::io::endLogger();
    std::cout << "Preprocessing completed" << std::endl;
}
