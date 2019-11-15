#pragma once

#include <fstream>
#include <sstream>
#include "genericInput.h"
#include "MPIConjugateGradientInversionInputCardReader.h"
#include "inversionInterface.h"
#include "forwardModelInterface.h"
#include "MPIConjugateGradientInversionInput.h"

using std::cout;
using std::endl;

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
    void getUpdateDirectionInformation();

public:

    MPIConjugateGradientInversion(ForwardModelInterface *forwardModel, const GenericInput& gInput);

    MPIConjugateGradientInversion(const MPIConjugateGradientInversion&) = delete;
    MPIConjugateGradientInversion& operator=(const MPIConjugateGradientInversion&) = delete;

    double findRealRootFromCubic(double a, double b, double c, double d);

    PressureFieldSerial Reconstruct(const std::vector<std::complex<double>> &pData, GenericInput gInput );
    void ReconstructSlave(GenericInput gInput);
};