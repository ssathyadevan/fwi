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

/**
 * @brief The ConjugateGradientInversion class uses the conjugate gradient method, as described in the README and the thesis by Peter Haffinger,
 * to iteratively estimate the underlying structure using pressure data measured by the receivers and a forward model, which is able to convert
 * an estimated structure to simulated pressure data.
 */

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

    /**
     * @brief ConjugateGradientInversion::OpenResidualLogFile Opens a logfile in which the residuals can be stored.
     * @param gInput is the general input of the model, containing the outputlocation and runName.
     * @return std::ofstream residualLogFile in which the residuals can be stored.
     */
    std::ofstream OpenResidualLogFile(GenericInput& gInput);

    /**
     * @brief calculateUpdateDirection uses eq: updateDirectionsCG from the README to determine the update-direction of each gridpoint of the contrast function
     * @param residualArray contains the residual for each combination of sources receivers and frequencies.
     * @param gradientCurrent is the gradient, determined using the conjugate-gradient method
     * @param eta is a scaling factor for the residual (eq: errorFuncSubEtaInv in the README)
     * @return PressureFieldSerial zeta is the update direction for each value in the contrast function
     */
    PressureFieldSerial calculateUpdateDirection(std::vector<std::complex<double>> &residualArray, PressureFieldSerial &gradientCurrent, const double eta);

    /**
     * @brief calculateStepSize uses equation eq: optimalStepSizeCG (README) to calculate the optimal stepsize based on the update-direction
     * @param zeta is the update direction for each value in the contrast function
     * @param residualArray contains the residual for each combination of sources receivers and frequencies.
     * @return double alpha, the optimum step size
     */
    double calculateStepSize(PressureFieldSerial& zeta, std::vector<std::complex<double>>& residualArray);

    /**
     * @brief errorFunctional calculates the residual according to equation eq: errorFunc(README)
     * @param residualArray contains the residual for each combination of sources receivers and frequencies.
     * @param pData Complex vector with the real measured data
     * @param eta is a scaling factor for the residual (eq: errorFuncSubEtaInv in the README)
     * @return double residual, the scaled and averaged residual over all sources/frequences/receivers
     */
    double errorFunctional(std::vector<std::complex<double>> &residualArray, const std::vector<std::complex<double>> &pData, double eta);

    /**
     * @brief logResidualResults saves the residuals in the logfile and in a separate file with iteration number for clarity.
     * @param it1 iteration-number of the inner loop
     * @param it iteration-number of the outer loop
     * @param residual is a scaled and averaged error over all sources/receivers/frequencies
     * @param counter iteration number
     * @param residualLogFile file where residuals are stored separately
     */
    void logResidualResults(int it1, int it, double residual, int counter, std::ofstream &residualLogFile);

    /**
     * @brief calculateRegularisationParameters calculates all regularisation parameters necessary for the calculations in the current loop.
     * @param regularisationPrevious struct containting the regularisation parameters of the previous loop
     * @param regularisationCurrent struct containting the regularisation parameters of the current loop
     * @param deltaAmplification
     * @see calculateSteeringFactor
     * @see calculateWeightingFactor
     * @see calculateRegularisationGradient
     */
    void calculateRegularisationParameters(RegularisationParameters &regularisationPrevious, RegularisationParameters &regularisationCurrent, double deltaAmplification);

    /**
     * @brief calculateSteeringFactor uses eq. 2.23 from the thesis to calculate the steering factor and stores it in the regularisationCurrent
     * @param regularisationPrevious struct containting the regularisation parameters of the previous loop
     * @param regularisationCurrent struct containting the regularisation parameters of the current loop
     * @param deltaAmplification
     */
    void calculateSteeringFactor(const RegularisationParameters &regularisationPrevious, RegularisationParameters &regularisationCurrent,  double deltaAmplification);

    /**
     * @brief calculateWeightingFactor uses eq. 2.22 from the thesis to calculate the weighting factor and storing it in regularisationCurrent
     * @param regularisationPrevious struct containting the regularisation parameters of the previous loop
     * @param regularisationCurrent struct containting the regularisation parameters of the current loop
     */
    void calculateWeightingFactor(const RegularisationParameters &regularisationPrevious, RegularisationParameters &regularisationCurrent);

    /**
     * @brief calculateRegularisationGradient uses equation 2.25 from the thesis to calculate the regularisation gradient and store it in regularisationCurrent
     * @param regularisationPrevious struct containting the regularisation parameters of the previous loop
     * @param regularisationCurrent struct containting the regularisation parameters of the current loop
     */
    void calculateRegularisationGradient(const RegularisationParameters &regularisationPrevious, RegularisationParameters &regularisationCurrent);

    /**
     * @brief calculateUpdateDirection_regularisation uses multiplicative regularisation parameters to calculate an update direction (zeta) which leads to faster convergence.
     * @param residualArray contains the residual for each combination of sources receivers and frequencies.
     * @param gradientCurrent is the gradient, determined using the conjugate-gradient method from the current loop
     * @param gradientPrevious is the gradient, determined using the conjugate-gradient method from the previous loop
     * @param is a scaling factor for the residual (eq: errorFuncSubEtaInv in the README)
     * @param regularisationCurrent struct containting the regularisation parameters of the current loop
     * @param regularisationPrevious struct containting the regularisation parameters of the previous loop
     * @param zeta is the previous update direction for each value in the contrast function
     * @param residualPrevious scaled and averaged error of the previous loop
     * @return PressureFieldSerial zeta
     */
    PressureFieldSerial calculateUpdateDirection_regularisation(std::vector<std::complex<double>> &residualArray, PressureFieldSerial &gradientCurrent, const PressureFieldSerial &gradientPrevious, const double eta, const RegularisationParameters &regularisationCurrent, const RegularisationParameters &regularisationPrevious, PressureFieldSerial &zeta, double residualPrevious);

    /**
     * @brief calculateStepSize_regularisation optimizes the total error functional by taking the derivative of equation 2.26 of the thesis with respect to alpha and settting to zero.
     * @param regularisationPrevious struct containting the regularisation parameters of the previous loop
     * @param regularisationCurrent struct containting the regularisation parameters of the current loop
     * @param nTotal total number of signals of sources/receivers/frequencies
     * @param residualArray contains the residual for each combination of sources receivers and frequencies.
     * @param eta is a scaling factor for the residual (eq: errorFuncSubEtaInv in the README)
     * @param fDataPrevious is the error functional without regularisation of the previous loop
     * @param zeta is the update direction for each value in the contrast function
     * @return double alpha, the optimal stepsize
     */
    double calculateStepSize_regularisation(const RegularisationParameters &regularisationPrevious, RegularisationParameters &regularisationCurrent, const int nTotal, const std::vector<std::complex<double>> &residualArray, const double eta, const double fDataPrevious, const PressureFieldSerial& zeta);

    /**
     * @brief findRealRootFromCubic assuming y = ax^3 + bx^2 +cx + d and assuming only one real root, this function finds the real root
     * using Cardano's formula
     * @param a parameter before x^3
     * @param b parameter before x^2
     * @param c parameter before x^1
     * @param d parameter before x^0
     * @return double x, where y = 0
     */
    double findRealRootFromCubic(double a, double b, double c, double d);

    /**
     * @brief calculateRegularisationErrorFunctional uses equation 2.21 from the thesis to calculate the error functional for regularisation and stores it in regularisationPrevious
     * @param regularisationPrevious
     * @param regularisationCurrent
     */
    void calculateRegularisationErrorFunctional(RegularisationParameters &regularisationPrevious, RegularisationParameters &regularisationCurrent);

public:

    ConjugateGradientInversion(ForwardModelInterface *forwardModel, const ConjugateGradientInversionInput& invInput);
    ConjugateGradientInversion(const ConjugateGradientInversion&) = delete;
    ConjugateGradientInversion& operator=(const ConjugateGradientInversion&) = delete;

    /**
     * @brief Reconstruct iteratively reconstructs the contrast function of the
     * underlying structure of the earth, using the conjugate gradient method.
     * @param pData Complex vector with the real measured data
     * @param gInput Struct containing general model parameters
     * @return PressureFieldSerial _chiEstimate, the optimized estimation for the contrast function.
     */
    PressureFieldSerial Reconstruct(const std::vector<std::complex<double>> &pData, GenericInput gInput );
};
