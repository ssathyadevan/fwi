#pragma once

#include <fstream>
#include <sstream>
#include "genericInput.h"
#include "conjugateGradientInversionInputCardReader.h"
#include "inversionInterface.h"
#include "forwardModelInterface.h"
#include "conjugateGradientInversionInput.h"

using std::cout;
using std::endl;

class ConjugateGradientInversion : public InversionInterface
{
private:
    ForwardModelInterface* _forwardModel;
    ConjugateGradientInversionInput _cgInput;

    const Grid2D& _grid;
    const Sources& _src;
    const Receivers& _recv;
    const FrequenciesGroup& _freq;
    
    double calculateAlpha(std::vector<std::complex<double>>& residuals, PressureFieldSerial& zeta);
    PressureFieldSerial calculateB(PressureFieldSerial** gradientChiNMin1);
    double calculateDeltaSquared(PressureFieldSerial** gradientChiNMin1)

public:

    ConjugateGradientInversion(ForwardModelInterface *forwardModel, const GenericInput& gInput);

    ConjugateGradientInversion(const ConjugateGradientInversion&) = delete;
    ConjugateGradientInversion& operator=(const ConjugateGradientInversion&) = delete;

    double findRealRootFromCubic(double a, double b, double c, double d);

    PressureFieldSerial Reconstruct(const std::vector<std::complex<double>> &pData, GenericInput gInput );
};