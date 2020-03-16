#pragma once

#include <fstream>
#include <sstream>
#include "genericInput.h"
#include "conjugateGradientInversionInputCardReader.h"
#include "inversionInterface.h"
#include "forwardModelInterface.h"
#include "conjugateGradientInversionInput.h"
#include "log.h"

class ConjugateGradientInversion : public InversionInterface
{
private:
    ForwardModelInterface* _forwardModel;
    ConjugateGradientInversionInput _cgInput;

    const Grid2D& _grid;
    const Sources& _sources;
    const Receivers& _receivers;
    const FrequenciesGroup& _frequencies;

    PressureFieldSerial _chiEstimate;

    double _previousLowPoint = std::numeric_limits<double>::max();

    double calculateAlpha(PressureFieldSerial& zeta, std::vector<std::complex<double>>& residuals);
    double calculateAlpha_regression(const std::vector<std::complex<double>>& zetaTemp, PressureFieldSerial **gradientZetaTmp, const int nTotal, const double deltasquaredOld, const PressureFieldSerial& b, const PressureFieldSerial& bsquared, const std::vector<std::complex<double>> &resArray, PressureFieldSerial **gradientChiOld, const double eta, const double fDataOld, const PressureFieldSerial& zeta);

    std::ofstream OpenResidualLogFile(GenericInput& gInput);
    double findRealRootFromCubic(double a, double b, double c, double d);

    void logResidualResults(int it1, int it, double error, int counter, std::ofstream &residualLogFile);
    
    double errorFunctional(std::vector<std::complex<double>> &residualArray, const std::vector<std::complex<double>> &pData, double eta);

    PressureFieldSerial determineRegularisationGradient(PressureFieldSerial **gradientChiOld, PressureFieldSerial **gradientGregTmp, PressureFieldSerial &bsquaredOld);

public:

    ConjugateGradientInversion(ForwardModelInterface *forwardModel, const ConjugateGradientInversionInput& invInput);

    ConjugateGradientInversion(const ConjugateGradientInversion&) = delete;
    ConjugateGradientInversion& operator=(const ConjugateGradientInversion&) = delete;


    PressureFieldSerial Reconstruct(const std::vector<std::complex<double>> &pData, GenericInput gInput );
    PressureFieldSerial ReconstructMPI(const std::vector<std::complex<double>> &pData, GenericInput gInput );
    void ReconstructMPISlave(GenericInput gInput);
};
