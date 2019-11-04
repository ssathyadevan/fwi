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

class conjugateGradientInversion : public inversionInterface
{
private:
    ForwardModelInterface* _forwardModel;
    conjugateGradientInversionInput _cgInput;

    const grid2D& _grid;
    const sources& _src;
    const receivers& _recv;
    const frequenciesGroup& _freq;
    double _previousLowPoint = std::numeric_limits<double>::max();

public:

    conjugateGradientInversion(ForwardModelInterface *forwardModel, const genericInput& gInput);

    conjugateGradientInversion(const conjugateGradientInversion&) = delete;
    conjugateGradientInversion& operator=(const conjugateGradientInversion&) = delete;

    double findRealRootFromCubic(double a, double b, double c, double d);

    pressureFieldSerial Reconstruct(const std::complex<double> *const pData, genericInput gInput );
};