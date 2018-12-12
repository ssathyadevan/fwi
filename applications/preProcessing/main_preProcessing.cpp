#include "read_input_fwi_into_vec.h"
#include "communication.h"
#include "sources_rect_2D.h"
#include "receivers_rect_2D.h"
#include "forwardModel.h"

int generateReferencePressureFieldFromChi(const Input& input);

int main(int argc, char** argv)
{
     Input input = reader3(argc, argv);

    if (!input.verbose)
    {
        WriteToFileNotToTerminal(input.outputLocation, input.cardName, "PreProcess");
    }
    ClockStart(input.freq.nTotal);
    int ret =generateReferencePressureFieldFromChi(input);

    ClockStop(ret);

    return 0;
}

int generateReferencePressureFieldFromChi (const Input& input)
{

    #include "setupInputParametersForFurtherCalculations.h"

    int magnitude = input.freq.nTotal * input.nSourcesReceivers.src * input.nSourcesReceivers.rec;

    std::complex<double> referencePressureData[magnitude];

    chi.toFile(input.outputLocation + "chi_ref_"+ input.cardName+ ".txt");

    ForwardModelInterface *forwardModel;
    forwardModel = new ForwardModel(grid, src, recv, freqg, *profiler, input);

    std::cout << "Calculate pData (the reference pressure-field)..." << std::endl;
    forwardModel->calculateData(referencePressureData, chi, input.conjGrad);

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
    
    return 0;
}


