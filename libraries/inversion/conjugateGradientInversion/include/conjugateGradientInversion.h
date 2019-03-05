#ifndef INVERSION_CPU
#define INVERSION_CPU

#include <fstream>
#include <sstream>
#include "inversionInterface.h"
#include "forwardModelBasicOptimization.h"
#include "conjugateGradientInput.h"

using std::cout;
using std::endl;

class conjugateGradientInversion : public inversionInterface
{
private:
    forwardModelBasicOptimization* m_forwardModel;
    conjugateGradientInput m_cgInput;

    const grid2D& m_grid;
    const sources& m_src;
    const receivers& m_recv;
    const frequenciesGroup& m_freq;


public:

    conjugateGradientInversion(forwardModelBasicOptimization *forwardModel, const conjugateGradientInput &cgInput);

    conjugateGradientInversion(const conjugateGradientInversion&) = delete;
    conjugateGradientInversion& operator=(const conjugateGradientInversion&) = delete;

    double findRealRootFromCubic(double a, double b, double c, double d);

    pressureFieldSerial Reconstruct(const std::complex<double> *const pData, genericInput gInput );
};

#endif // INVERSION_CPU

