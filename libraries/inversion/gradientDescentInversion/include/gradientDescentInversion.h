#pragma once

#include "inversionInterface.h"
#include "forwardModelInterface.h"
#include "gradientDescentInversionInput.h"
#include "genericInput.h"
class gradientDescentInversion : public inversionInterface{

private:

    ForwardModelInterface* _forwardModel;
    gradientDescentInversionInput _gdInput;

    const grid2D& _grid;
    const sources& _src;
    const receivers& _recv;
    const frequenciesGroup _freq;

 public:
    gradientDescentInversion(ForwardModelInterface *forwardModel, const genericInput &gdInput);

    gradientDescentInversion(const gradientDescentInversion&) = delete;
    gradientDescentInversion & operator = (const gradientDescentInversion&) = delete;

    pressureFieldSerial Reconstruct(const std::complex<double> *const pData, genericInput gInput );
    std::vector<double> differential(const std::complex<double> *const pData, pressureFieldSerial xi, double dxi, double eta);
    pressureFieldSerial gradientDescent(const std::complex<double> *const pData, pressureFieldSerial xi, std::vector<double> nablaFxi, double gamma,  double eta);
    double functionF(pressureFieldSerial xi, const std::complex<double> *const pData, double eta);
    double getGamma(std::vector<double> dFdxiOld, std::vector<double> dFdxi, pressureFieldSerial xiOld, pressureFieldSerial xi);
};
