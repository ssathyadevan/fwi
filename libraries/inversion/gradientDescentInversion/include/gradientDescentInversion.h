#pragma once

#include "forwardModelInterface.h"
#include "genericInput.h"
#include "gradientDescentInversionInput.h"
#include "inversionInterface.h"
class gradientDescentInversion : public inversionInterface
{
private:
    forwardModelInterface *_forwardModel;
    gradientDescentInversionInput _gdInput;

    const grid2D &_grid;
    const sources &_src;
    const receivers &_recv;
    const frequenciesGroup _freq;

    pressureFieldSerial gradientDescent(pressureFieldSerial chiEstimate, const std::vector<double> &dfdx, const double gamma);

public:
    gradientDescentInversion(forwardModelInterface *forwardModel, const gradientDescentInversionInput &gdInput);

    gradientDescentInversion(const gradientDescentInversion &) = delete;
    gradientDescentInversion &operator=(const gradientDescentInversion &) = delete;

    pressureFieldSerial reconstruct(const std::vector<std::complex<double>> &pData, genericInput gInput);
    std::vector<double> differential(const std::vector<std::complex<double>> &pData, pressureFieldSerial xi, double dxi, double eta);
    double functionF(const pressureFieldSerial xi, const std::vector<std::complex<double>> &pData, double eta);
    double determineGamma(const pressureFieldSerial chiEstimatePrevious, const pressureFieldSerial chiEstimateCurrent, std::vector<double> dFdxPrevious,
        std::vector<double> dFdx);
};
