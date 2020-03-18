#pragma once

#include <fstream>
#include <sstream>
#include "genericInput.h"
#include "conjugateGradientInversionInputCardReader.h"
#include "inversionInterface.h"
#include "forwardModelInterface.h"
#include "conjugateGradientInversionInput.h"
#include "log.h"
#include "regularization.h"

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

    double calculateStepSize(PressureFieldSerial& zeta, std::vector<std::complex<double>>& residuals);
    double calculateStepSize_regularisation(const RegularisationParameters &regularisationPrevious, RegularisationParameters &regularisationCurrent, const int nTotal, const std::vector<std::complex<double>> &resArray, const double eta, const double fDataOld, const PressureFieldSerial& zeta);

    double errorFunctional(std::vector<std::complex<double>> &residualArray, const std::vector<std::complex<double>> &pData, double eta);

    void calculateRegularisationParameters(RegularisationParameters &regularisationPrevious, RegularisationParameters &regularisationCurrent, double deltaAmplification);
    void calculateSteeringFactor(const RegularisationParameters &regularisationPrevious, RegularisationParameters &regularisationCurrent,  double deltaAmplification);
    void calculateWeightingFactor(const RegularisationParameters &regularisationPrevious, RegularisationParameters &regularisationCurrent);
    void calculateRegularisationGradient(const RegularisationParameters &regularisationPrevious, RegularisationParameters &regularisationCurrent);


    std::ofstream OpenResidualLogFile(GenericInput& gInput);
    double findRealRootFromCubic(double a, double b, double c, double d);

    void logResidualResults(int it1, int it, double error, int counter, std::ofstream &residualLogFile);
    


    PressureFieldSerial calculateUpdateDirection(std::vector<std::complex<double>> &residualArray, PressureFieldSerial &gradientCurrent, const double eta);
    PressureFieldSerial calculateUpdateDirection_regularisation(std::vector<std::complex<double>> &residualArray, PressureFieldSerial &gradientCurrent, const PressureFieldSerial &gradientPrevious, const double eta, const RegularisationParameters &regularisationCurrent, const RegularisationParameters &regularisationPrevious, PressureFieldSerial &zeta, double residualPrevious);

    void calculateRegularisationErrorFunctional(RegularisationParameters &regularisationPrevious, RegularisationParameters &regularisationCurrent);

public:

    ConjugateGradientInversion(ForwardModelInterface *forwardModel, const ConjugateGradientInversionInput& invInput);
    ConjugateGradientInversion(const ConjugateGradientInversion&) = delete;

    ConjugateGradientInversion& operator=(const ConjugateGradientInversion&) = delete;

    PressureFieldSerial Reconstruct(const std::vector<std::complex<double>> &pData, GenericInput gInput );
    PressureFieldSerial ReconstructMPI(const std::vector<std::complex<double>> &pData, GenericInput gInput );
    void ReconstructMPISlave(GenericInput gInput);
};
