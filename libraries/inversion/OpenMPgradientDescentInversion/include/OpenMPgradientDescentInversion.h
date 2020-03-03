#pragma once

#include "inversionInterface.h"
#include "forwardModelInterface.h"
#include "gradientDescentInversionInput.h"
#include "genericInput.h"
class OpenMPGradientDescentInversion : public InversionInterface{

private:

    ForwardModelInterface* _forwardModel;
    GradientDescentInversionInput _gdInput;

    const Grid2D& _grid;
    const Sources& _src;
    const Receivers& _recv;
    const FrequenciesGroup _freq;

    PressureFieldSerial gradientDescent(PressureFieldSerial xi, std::vector<double> nablaFxi, double gamma);
    void differential_parallel(const std::vector<std::complex<double>> &pData, const PressureFieldSerial xi, const double dxi, const double eta, const unsigned int blocksize, const unsigned int offset, std::vector<double>& my_dFdx);
    
 public:
    OpenMPGradientDescentInversion(ForwardModelInterface *forwardModel, const GenericInput &gdInput);

    OpenMPGradientDescentInversion(const OpenMPGradientDescentInversion&) = delete;
    OpenMPGradientDescentInversion & operator = (const OpenMPGradientDescentInversion&) = delete;

    PressureFieldSerial Reconstruct(const std::vector<std::complex<double>> &pData, GenericInput gInput );
    std::vector<double> differential(const std::vector<std::complex<double>> &pData, const PressureFieldSerial xi, const double dxi, const double eta);
    double functionF(PressureFieldSerial xi, const std::vector<std::complex<double>> &pData, double eta);
    double determineGamma(std::vector<double> dFdxPrevious, std::vector<double> dFdx, PressureFieldSerial xPrevious, PressureFieldSerial x);
};
