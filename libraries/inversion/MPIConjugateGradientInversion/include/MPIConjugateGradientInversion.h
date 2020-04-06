#pragma once

#include <fstream>
#include <sstream>
#include "genericInput.h"
#include "MPIConjugateGradientInversionInputCardReader.h"
#include "inversionInterface.h"
#include "forwardModelInterface.h"
#include "MPIConjugateGradientInversionInput.h"

class MPIConjugateGradientInversion : public InversionInterface
{
private:
    ForwardModelInterface* _forwardModel;
    MPIConjugateGradientInversionInput _cgInput;

    const Grid2D& _grid;
    const Sources& _src;
    const Receivers& _recv;
    const FrequenciesGroup& _freq;
    double _previousLowPoint = std::numeric_limits<double>::max();

    double calculateAlpha(PressureFieldSerial& zeta, std::vector<std::complex<double>>& residuals);
    PressureFieldSerial* getUpdateDirectionInformation(std::vector<std::complex<double>> resArray, const int mpi_size);

public:

    MPIConjugateGradientInversion(ForwardModelInterface *forwardModel, const MPIConjugateGradientInversionInput& cgInput);

    MPIConjugateGradientInversion(const MPIConjugateGradientInversion&) = delete;
    MPIConjugateGradientInversion& operator=(const MPIConjugateGradientInversion&) = delete;

    double findRealRootFromCubic(double a, double b, double c, double d);

    PressureFieldSerial Reconstruct(const std::vector<std::complex<double>> &pData, GenericInput gInput );
    void ReconstructSlave();
};
