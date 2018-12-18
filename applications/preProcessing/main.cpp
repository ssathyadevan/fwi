#include "read_input_fwi_into_vec.h"
#include "utilityFunctions.h"
#include "forwardModel.h"
#include "cpuClock.h"


void generateReferencePressureFieldFromChi(const Input& input);

int main(int argc, char** argv)
{
     Input input = reader3(argc, argv);

    if (!input.verbose)
    {
        WriteToFileNotToTerminal(input.outputLocation, input.cardName, "PreProcess");
    }
    std::cout << "Preprocessing the provided input to create the reference pressure-field" << std::endl;

    cpuClock clock;

    clock.Start();
    generateReferencePressureFieldFromChi(input);
    clock.End();
    clock.PrintTimeElapsed();

    return 0;
}

void generateReferencePressureFieldFromChi (const Input& input)
{
    // initialize the grid, sources, receivers, grouped frequencies
    grid_rect_2D grid(input.reservoirTopLeftCornerInM, input.reservoirBottomRightCornerInM, input.ngrid);
    volField_rect_2D_cpu chi(grid);
    chi.fromFile(input);
    Sources_rect_2D src(input.sourcesTopLeftCornerInM, input.sourcesBottomRightCornerInM, input.nSourcesReceivers.src);
    src.Print();
    Receivers_rect_2D recv(src);
    recv.Print();
    Frequencies_group freqg(input.freq, input.c_0);
    freqg.Print(input.freq.nTotal);

    int magnitude = input.freq.nTotal * input.nSourcesReceivers.src * input.nSourcesReceivers.rec;

    std::complex<double> referencePressureData[magnitude];

    chi.toFile(input.outputLocation + "chi_ref_"+ input.cardName+ ".txt");

    ForwardModelInterface *forwardModel;
    forwardModel = new ForwardModel(grid, src, recv, freqg, input);

    std::cout << "Calculate pData (the reference pressure-field)..." << std::endl;
    forwardModel->calculateData(referencePressureData, chi, input.iter2);

    // writing the referencePressureData to a text file in complex form
    std::string invertedChiToPressureFileName = input.outputLocation + input.cardName + "InvertedChiToPressure.txt";
    std::ofstream file;
    file.open (invertedChiToPressureFileName, std::ios::out | std::ios::trunc);
    assert(file.is_open());
    for(int i=0; i < magnitude; i++)
    {
        file << std::setprecision(17) << referencePressureData[i].real()
             <<"," << referencePressureData[i].imag() << std::endl;
    }
    file.close();
    delete forwardModel;

}


