#include "conjugateGradientInversion.h"
#include "CommonVectorOperations.h"
#include "log.h"
#include "progressBar.h"
#include <iostream>
#include <memory>

#define TAG_COMMAND 0
#define COMMAND_GETUPDATEDIRECTION 1
#define COMMAND_EXIT -99
#define TAG_SIZE 1
#define TAG_RESARRAY 2
#define TAG_RESULT 3

using fwi::core::operator-;

namespace fwi
{
    namespace inversionMethods
    {
        ConjugateGradientInversion::ConjugateGradientInversion(const core::CostFunctionCalculator &costCalculator,
            forwardModels::ForwardModelInterface *forwardModel, const ConjugateGradientInversionInput &invInput)
            : _forwardModel(forwardModel)
            , _costCalculator(costCalculator)
            , _cgInput(invInput)
            , _grid(forwardModel->getGrid())
            , _sources(forwardModel->getSource())
            , _receivers(forwardModel->getReceiver())
            , _frequencies(forwardModel->getFreq())
            , _chiEstimate(_grid)
        {
        }

        core::dataGrid2D ConjugateGradientInversion::reconstruct(const std::vector<std::complex<double>> &pData, io::genericInput gInput)
        {
            io::progressBar progressBar(_cgInput.n_max * _cgInput.iteration1.n);
            const double eta = 1.0 / _costCalculator.calculateCost(pData, std::vector<std::complex<double>>(pData.size(), 0.0), 1.0);
            _chiEstimate.zero();

            std::ofstream residualLogFile = openResidualLogFile(gInput);
            bool isConverged = false;

            // main loop//
            int counter = 1;
            for(int it = 0; it < _cgInput.n_max; it++)
            {
                // Initialize conjugate gradient parameters
                core::dataGrid2D gradientCurrent(_grid), gradientPrevious(_grid), zeta(_grid);
                double residualCurrent = 0.0, residualPrevious = 0.0, alpha = 0.0;

                _forwardModel->calculateKappa();
                auto pDataEst = _forwardModel->calculatePressureField(_chiEstimate);
                std::vector<std::complex<double>> residualArray = pData - pDataEst;

                // Initialize Regularisation parameters
                double deltaAmplification = _cgInput.dAmplification.start / (_cgInput.dAmplification.slope * it + 1.0);
                // Note: deltaAmplification decreases the step size for increasing iteration step

                RegularisationParameters regularisationCurrent(_grid);
                RegularisationParameters regularisationPrevious(_grid);
                regularisationPrevious.bSquared.zero();

                // First iteration
                //------------------------------------------------------------------
                zeta = calculateUpdateDirection(residualArray, gradientCurrent, eta);
                alpha = calculateStepSize(zeta, residualArray);   // eq: optimalStepSizeCG

                // Update contrast-function
                _chiEstimate += alpha * zeta;   // eq: contrastUpdate

                // Result + logging
                pDataEst = _forwardModel->calculatePressureField(_chiEstimate);
                residualCurrent = _costCalculator.calculateCost(pData, pDataEst, eta);   // eq: errorFunc
                isConverged = (residualCurrent < _cgInput.iteration1.tolerance);
                logResidualResults(0, it, residualCurrent, counter, residualLogFile, isConverged);

                // Save variables for next iteration
                gradientPrevious = gradientCurrent;
                residualPrevious = residualCurrent;

                counter++;
                // -----------------------------------------------------------------

                // start the inner loop
                for(int it1 = 1; it1 < _cgInput.iteration1.n; it1++)
                {
                    calculateRegularisationParameters(regularisationPrevious, regularisationCurrent, deltaAmplification);

                    zeta = calculateUpdateDirectionRegularisation(residualArray, gradientCurrent, gradientPrevious, eta, regularisationCurrent,
                        regularisationPrevious, zeta, residualPrevious);   // eq: updateDirectionsCG
                    alpha = calculateStepSizeRegularisation(regularisationPrevious, regularisationCurrent, residualArray, eta, residualPrevious, zeta);

                    // Update contrast-function
                    _chiEstimate += alpha * zeta;

                    // Result + logging
                    pDataEst = _forwardModel->calculatePressureField(_chiEstimate);
                    residualCurrent = _costCalculator.calculateCost(pData, pDataEst, eta);
                    isConverged = (residualCurrent < _cgInput.iteration1.tolerance);
                    logResidualResults(it1, it, residualCurrent, counter, residualLogFile, isConverged);

                    // breakout check
                    if((it1 > 0) &&
                        ((residualCurrent < _cgInput.iteration1.tolerance) || (std::abs(residualPrevious - residualCurrent) < _cgInput.iteration1.tolerance)))
                    {
                        progressBar.setCounter(_cgInput.iteration1.n + progressBar.getCounter() - (progressBar.getCounter() % _cgInput.iteration1.n));
                        break;
                    }

                    // Save variables for next iteration
                    gradientPrevious = gradientCurrent;
                    residualPrevious = residualCurrent;

                    // Save regularisation variables for next iteration
                    _chiEstimate.gradient(regularisationCurrent.gradientChi);
                    calculateRegularisationErrorFunctional(regularisationPrevious, regularisationCurrent);

                    regularisationPrevious.deltaSquared = regularisationCurrent.deltaSquared;
                    regularisationPrevious.bSquared = regularisationCurrent.bSquared;

                    counter++;
                    progressBar++;
                }
            }
            residualLogFile.close();

            return _chiEstimate;
        }

        std::ofstream ConjugateGradientInversion::openResidualLogFile(io::genericInput &gInput)
        {
            std::string filePath = gInput.outputLocation + gInput.runName + "Residual" + ".log";

            std::ofstream residualLogFile;
            residualLogFile.open(filePath, std::ios::out | std::ios::trunc);
            if(!residualLogFile)
            {
                throw std::invalid_argument("Unable to store residuals from file : " + filePath);
            }

            return residualLogFile;
        }

        core::dataGrid2D ConjugateGradientInversion::calculateUpdateDirection(
            std::vector<std::complex<double>> &residualArray, core::dataGrid2D &gradientCurrent, double eta)
        {
            core::complexDataGrid2D kappaTimesResidual(_grid);   // eq: integrandForDiscreteK of README, KappaTimesResidual is the argument of Re()
            _forwardModel->getUpdateDirectionInformation(residualArray, kappaTimesResidual);
            gradientCurrent = eta * kappaTimesResidual.getRealPart();

            return gradientCurrent;
        }

        double ConjugateGradientInversion::calculateStepSize(const core::dataGrid2D &zeta, std::vector<std::complex<double>> &residualArray)
        {
            double alphaNumerator = 0.0;
            double alphaDenominator = 0.0;

            std::vector<std::complex<double>> kappaTimesZeta = _forwardModel->calculatePressureField(zeta);

            for(size_t i = 0; i < kappaTimesZeta.size(); i++)
            {
                alphaNumerator += std::real(conj(residualArray[i]) * kappaTimesZeta[i]);
                alphaDenominator += std::real(conj(kappaTimesZeta[i]) * kappaTimesZeta[i]);
            }

            if(alphaDenominator == 0.0)
            {
                throw std::overflow_error("ConjugateGradient: the computation of alpha devides by zero.");
            }
            double alpha = alphaNumerator / alphaDenominator;

            return alpha;
        }

        void ConjugateGradientInversion::logResidualResults(int it1, int it, double residual, int counter, std::ofstream &residualLogFile, bool isConverged)
        {
            std::string convergenceMessage = isConverged ? "Converged" : "Not Converged";
            L_(io::linfo) << it1 + 1 << "/" << _cgInput.iteration1.n << "\t (" << it + 1 << "/" << _cgInput.n_max << ")\t res: " << std::setprecision(17)
                          << residual << "\t" << convergenceMessage;
            residualLogFile << std::setprecision(17) << residual << "," << counter << std::endl;
        }

        void ConjugateGradientInversion::calculateRegularisationParameters(
            RegularisationParameters &regularisationPrevious, RegularisationParameters &regularisationCurrent, double deltaAmplification)
        {
            _chiEstimate.gradient(regularisationPrevious.gradientChi);

            regularisationPrevious.gradientChiNormSquared = (regularisationPrevious.gradientChi[0] * regularisationPrevious.gradientChi[0]) +
                                                            (regularisationPrevious.gradientChi[1] * regularisationPrevious.gradientChi[1]);

            regularisationCurrent.bSquared = calculateWeightingFactor(regularisationPrevious);   // # eq. 2.22
            regularisationCurrent.b = regularisationCurrent.bSquared;
            regularisationCurrent.b.sqrt();

            regularisationCurrent.deltaSquared = calculateSteeringFactor(regularisationPrevious, regularisationCurrent, deltaAmplification);   // eq. 2.23
            regularisationCurrent.gradient = calculateRegularisationGradient(regularisationPrevious);
        }

        core::dataGrid2D ConjugateGradientInversion::calculateWeightingFactor(const RegularisationParameters &regularisationPrevious)
        {
            core::dataGrid2D bSquared(_grid);
            bSquared = regularisationPrevious.gradientChiNormSquared + regularisationPrevious.deltaSquared;
            bSquared.reciprocal();
            bSquared *= (1.0 / (_grid.getDomainArea()));
            return bSquared;
        }

        double ConjugateGradientInversion::calculateSteeringFactor(
            const RegularisationParameters &regularisationPrevious, const RegularisationParameters &regularisationCurrent, double deltaAmplification)
        {
            core::dataGrid2D bTimesGradientChiXSquared = regularisationCurrent.b * regularisationPrevious.gradientChi[0];
            bTimesGradientChiXSquared.square();
            core::dataGrid2D bTimesGradientChiZSquared = regularisationCurrent.b * regularisationPrevious.gradientChi[1];
            bTimesGradientChiZSquared.square();

            double bTimesGradientChiNormSquared = (bTimesGradientChiXSquared + bTimesGradientChiZSquared).summation();
            double bSquaredSummed = regularisationCurrent.bSquared.summation();

            return deltaAmplification * 0.5 * bTimesGradientChiNormSquared / bSquaredSummed;
        }

        core::dataGrid2D ConjugateGradientInversion::calculateRegularisationGradient(const RegularisationParameters &regularisationPrevious)
        {
            std::vector<core::dataGrid2D> temporaryGradient(2, core::dataGrid2D(_grid));

            core::dataGrid2D bSquaredTimesGradientChiX = regularisationPrevious.bSquared * regularisationPrevious.gradientChi[0];
            bSquaredTimesGradientChiX.gradient(temporaryGradient);
            core::dataGrid2D gradientBSquaredTimesGradientChiX = temporaryGradient[0];

            core::dataGrid2D bSquaredTimesGradientChiZ = regularisationPrevious.bSquared * regularisationPrevious.gradientChi[1];
            bSquaredTimesGradientChiZ.gradient(temporaryGradient);
            core::dataGrid2D gradientBSquaredTimesGradientChiZ = temporaryGradient[1];

            return gradientBSquaredTimesGradientChiX + gradientBSquaredTimesGradientChiZ;
        }

        core::dataGrid2D ConjugateGradientInversion::calculateUpdateDirectionRegularisation(std::vector<std::complex<double>> &residualArray,
            core::dataGrid2D &gradientCurrent, const core::dataGrid2D &gradientPrevious, const double eta,
            const RegularisationParameters &regularisationCurrent, const RegularisationParameters &regularisationPrevious, const core::dataGrid2D &zeta,
            double residualPrevious)
        {
            core::complexDataGrid2D kappaTimesResidual(_grid);
            kappaTimesResidual.zero();
            _forwardModel->getUpdateDirectionInformation(residualArray, kappaTimesResidual);
            gradientCurrent = eta * regularisationPrevious.errorFunctional * kappaTimesResidual.getRealPart() +
                              residualPrevious * regularisationCurrent.gradient;   // eq: 2.25 of thesis
            double gamma = gradientCurrent.innerProduct(gradientCurrent - gradientPrevious) /
                           gradientPrevious.innerProduct(gradientPrevious);   // eq: PolakRibiereDirection README

            return gradientCurrent + gamma * zeta;
        }

        double ConjugateGradientInversion::calculateStepSizeRegularisation(const RegularisationParameters &regularisationPrevious,
            RegularisationParameters &regularisationCurrent, const std::vector<std::complex<double>> &residualArray, const double eta,
            const double fDataPrevious, const core::dataGrid2D &zeta)
        {
            std::vector<std::complex<double>> kappaTimesZeta = _forwardModel->calculatePressureField(zeta);

            double a0 = fDataPrevious;

            double a1 = 0.0;
            double a2 = 0.0;

            for(size_t i = 0; i < kappaTimesZeta.size(); i++)
            {
                a1 += -2.0 * eta * std::real(conj(residualArray[i]) * kappaTimesZeta[i]);
                a2 += eta * std::real(conj(kappaTimesZeta[i]) * kappaTimesZeta[i]);
            }

            core::dataGrid2D bGradientChiSquaredXDirection =
                (regularisationCurrent.b * regularisationPrevious.gradientChi[0]) * (regularisationCurrent.b * regularisationPrevious.gradientChi[0]);
            core::dataGrid2D bGradientChiSquaredZDirection =
                (regularisationCurrent.b * regularisationPrevious.gradientChi[1]) * (regularisationCurrent.b * regularisationPrevious.gradientChi[1]);
            double b0 = ((bGradientChiSquaredXDirection.summation() + bGradientChiSquaredZDirection.summation()) +
                            regularisationPrevious.deltaSquared * regularisationCurrent.bSquared.summation()) *
                        _grid.getCellVolume();

            std::vector<core::dataGrid2D> gradientZeta(2, core::dataGrid2D(_grid));
            zeta.gradient(gradientZeta);

            core::dataGrid2D bGradientZetabGradientChiX =
                (regularisationCurrent.b * gradientZeta[0]) * (regularisationCurrent.b * regularisationPrevious.gradientChi[0]);
            core::dataGrid2D bGradientZetabGradientChiZ =
                (regularisationCurrent.b * gradientZeta[1]) * (regularisationCurrent.b * regularisationPrevious.gradientChi[1]);
            double b1 = 2.0 * (bGradientZetabGradientChiX.summation() + bGradientZetabGradientChiZ.summation()) * _grid.getCellVolume();

            core::dataGrid2D bTimesGradientZetaXdirection = regularisationCurrent.b * gradientZeta[0];
            core::dataGrid2D bTimesGradientZetaZdirection = regularisationCurrent.b * gradientZeta[1];
            bTimesGradientZetaXdirection.square();
            bTimesGradientZetaZdirection.square();
            double b2 = (bTimesGradientZetaXdirection.summation() + bTimesGradientZetaZdirection.summation()) * _grid.getCellVolume();

            double derA = 4.0 * a2 * b2;
            double derB = 3.0 * (a2 * b1 + a1 * b2);
            double derC = 2.0 * (a2 * b0 + a1 * b1 + a0 * b2);
            double derD = a1 * b0 + a0 * b1;

            return findRealRootFromCubic(derA, derB, derC, derD);
        }

        double ConjugateGradientInversion::findRealRootFromCubic(double a, double b, double c, double d)
        {
            double f = ((3.0 * c / a) - (std::pow(b, 2) / std::pow(a, 2))) / 3.0;
            double g = ((2.0 * std::pow(b, 3) / std::pow(a, 3)) - (9.0 * b * c / std::pow(a, 2)) + (27.0 * d / a)) / 27.0;
            double h = (std::pow(g, 2) / 4.0) + (std::pow(f, 3) / 27.0);
            double r = -(g / 2.0) + std::sqrt(h);
            double s = std::cbrt(r);
            double t = -(g / 2.0) - std::sqrt(h);
            double u = std::cbrt(t);

            double realRoot = s + u - (b / (3.0 * a));

            return realRoot;
        }

        void ConjugateGradientInversion::calculateRegularisationErrorFunctional(
            RegularisationParameters &regularisationPrevious, RegularisationParameters &regularisationCurrent)
        {
            core::dataGrid2D gradientChiNormsquaredCurrent(_grid);
            gradientChiNormsquaredCurrent = (regularisationCurrent.gradientChi[0] * regularisationCurrent.gradientChi[0]) +
                                            (regularisationCurrent.gradientChi[1] * regularisationCurrent.gradientChi[1]);

            core::dataGrid2D integral = (gradientChiNormsquaredCurrent + regularisationPrevious.deltaSquared) /
                                        (regularisationPrevious.gradientChiNormSquared + regularisationPrevious.deltaSquared);

            regularisationPrevious.errorFunctional = (1.0 / (_grid.getDomainArea())) * integral.summation() * _grid.getCellVolume();
        }
    }   // namespace inversionMethods
}   // namespace fwi
