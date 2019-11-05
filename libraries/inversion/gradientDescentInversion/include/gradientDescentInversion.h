#pragma once

#include "inversionInterface.h"
#include "forwardModelInterface.h"
#include "gradientDescentInversionInput.h"
#include "genericInput.h"
class GradientDescentInversion : public InversionInterface{

private:

    ForwardModelInterface* _forwardModel;
    GradientDescentInversionInput _gdInput;

    const grid2D& _grid;
    const sources& _src;
    const receivers& _recv;
    const frequenciesGroup _freq;

 public:
    GradientDescentInversion(ForwardModelInterface *forwardModel, const genericInput &gdInput);

    GradientDescentInversion(const GradientDescentInversion&) = delete;
    GradientDescentInversion & operator = (const GradientDescentInversion&) = delete;

    PressureFieldSerial Reconstruct(const std::complex<double> *const pData, genericInput gInput );
    std::vector<double> differential(const std::complex<double> *const pData, pressureFieldSerial xi, double dxi, double eta);
    PressureFieldSerial gradientDescent(const std::complex<double> *const pData, pressureFieldSerial xi, std::vector<double> nablaFxi, double gamma,  double eta);
    double functionF(pressureFieldSerial xi, const std::complex<double> *const pData, double eta);
    double determineGamma(std::vector<double> dFdxiOld, std::vector<double> dFdxi, pressureFieldSerial xiOld, pressureFieldSerial xi);
};
