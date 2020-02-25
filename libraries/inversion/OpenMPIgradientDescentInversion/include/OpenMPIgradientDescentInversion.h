#pragma once

#include "inversionInterface.h"
#include "forwardModelInterface.h"
#include "gradientDescentInversionInput.h"
#include "genericInput.h"
class OpenMPIGradientDescentInversion : public InversionInterface{

private:

    ForwardModelInterface* _forwardModel;
    GradientDescentInversionInput _gdInput;

    const Grid2D& _grid;
    const Sources& _src;
    const Receivers& _recv;
    const FrequenciesGroup _freq;

    PressureFieldSerial gradientDescent(PressureFieldSerial xi, std::vector<double> nablaFxi, double gamma);
    void differential_parallel(const std::vector<std::complex<double>> &pData, PressureFieldSerial xi, double dxi, double eta, int blocksize, int offset, std::vector<double>& my_dFdx);
    
 public:
    OpenMPIGradientDescentInversion(ForwardModelInterface *forwardModel, const GenericInput &gdInput);

    OpenMPIGradientDescentInversion(const OpenMPIGradientDescentInversion&) = delete;
    OpenMPIGradientDescentInversion & operator = (const OpenMPIGradientDescentInversion&) = delete;

    PressureFieldSerial Reconstruct(const std::vector<std::complex<double>> &pData, GenericInput gInput );
    std::vector<double> differential(const std::vector<std::complex<double>> &pData, PressureFieldSerial xi, double dxi, double eta);
    double functionF(PressureFieldSerial xi, const std::vector<std::complex<double>> &pData, double eta);
    double determineGamma(std::vector<double> dFdxPrevious, std::vector<double> dFdx, PressureFieldSerial xPrevious, PressureFieldSerial x);
};
