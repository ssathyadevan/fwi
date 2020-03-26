#pragma once

#include "forwardModelInterface.h"
#include "genericInput.h"
#include "gradientDescentInversionInput.h"
#include "inversionInterface.h"
class GradientDescentInversion : public InversionInterface
{
private:
    ForwardModelInterface *_forwardModel;
    GradientDescentInversionInput _gdInput;

    const Grid2D &_grid;
    const Sources &_src;
    const Receivers &_recv;
    const FrequenciesGroup _freq;

    PressureFieldSerial gradientDescent(PressureFieldSerial chiEstimate, const std::vector<double> &dfdx, const double gamma);

public:
    GradientDescentInversion(ForwardModelInterface *forwardModel, const GradientDescentInversionInput &gdInput);

    GradientDescentInversion(const GradientDescentInversion &) = delete;
    GradientDescentInversion &operator=(const GradientDescentInversion &) = delete;

    PressureFieldSerial Reconstruct(const std::vector<std::complex<double>> &pData, GenericInput gInput);
    std::vector<double> differential(const std::vector<std::complex<double>> &pData, PressureFieldSerial xi, double dxi, double eta);
    double functionF(const PressureFieldSerial xi, const std::vector<std::complex<double>> &pData, double eta);
    double determineGamma(const PressureFieldSerial chiEstimatePrevious, const PressureFieldSerial chiEstimateCurrent, std::vector<double> dFdxPrevious,
        std::vector<double> dFdx);
};
