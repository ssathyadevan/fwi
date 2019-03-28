#ifndef INVERSION_CPU
#define INVERSION_CPU

#include <fstream>
#include <sstream>
#include "inversionInterface.h"
#include "IntegralForwardModel.h"
#include "conjugateGradientInput.h"

using std::cout;
using std::endl;

class conjugateGradientInversion : public inversionInterface
{
private:
    ForwardModelInterface* _forwardModel;
    conjugateGradientInput _cgInput;

    const grid2D& _grid;
    const sources& _src;
    const receivers& _recv;
    const frequenciesGroup& _freq;


public:

    conjugateGradientInversion(ForwardModelInterface *forwardModel, const conjugateGradientInput &cgInput);

    conjugateGradientInversion(const conjugateGradientInversion&) = delete;
    conjugateGradientInversion& operator=(const conjugateGradientInversion&) = delete;

    double findRealRootFromCubic(double a, double b, double c, double d);

    pressureFieldSerial Reconstruct(const std::complex<double> *const pData, genericInput gInput );
};

#endif // INVERSION_CPU

