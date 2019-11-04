#pragma once

#include "inversionInterface.h"
#include "forwardModelInterface.h"
#include "gradientDescentInversionInput.h"
#include "genericInput.h"
class GradientDescentInversion : public InversionInterface{

private:

    ForwardModelInterface* _forwardModel;
    GradientDescentInversionInput _gdInput;

    const Grid2D& _grid;
    const Sources& _src;
    const Receivers& _recv;
    const FrequenciesGroup _freq;

 public:
    GradientDescentInversion(ForwardModelInterface *forwardModel, const GenericInput &gdInput);

    GradientDescentInversion(const GradientDescentInversion&) = delete;
    GradientDescentInversion & operator = (const GradientDescentInversion&) = delete;

    PressureFieldSerial Reconstruct(const std::vector<std::complex<double>> &pData, GenericInput gInput );
    std::vector<double> differential(const std::vector<std::complex<double>> &pData, PressureFieldSerial xi, double dxi, double eta);
    PressureFieldSerial gradientDescent(const std::vector<std::complex<double>> &pData, PressureFieldSerial xi, std::vector<double> nablaFxi, double gamma,  double eta);
    double functionF(PressureFieldSerial xi, const std::vector<std::complex<double>> &pData, double eta);
    double getGamma(std::vector<double> dFdxiOld, std::vector<double> dFdxi, PressureFieldSerial xiOld, PressureFieldSerial xi);
};
