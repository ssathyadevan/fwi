#include "conjugateGradientInversion.h"
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

ConjugateGradientInversion::ConjugateGradientInversion(ForwardModelInterface *forwardModel, const ConjugateGradientInversionInput &invInput) :
    _forwardModel(), _cgInput(invInput), _grid(forwardModel->getGrid()), _sources(forwardModel->getSrc()), _receivers(forwardModel->getRecv()),
    _frequencies(forwardModel->getFreq()), _chiEstimate(_grid)
{
    _forwardModel = forwardModel;
}

PressureFieldSerial ConjugateGradientInversion::Reconstruct(const std::vector<std::complex<double>> &pData, GenericInput gInput)
{
    ProgressBar progressBar(_cgInput.n_max * _cgInput.iteration1.n);

    const int nTotal = _frequencies.nFreq * _sources.nSrc * _receivers.nRecv;
    const double eta = 1.0 / (normSq(pData, nTotal));   // Scaling factor
    _chiEstimate.Zero();

    std::ofstream residualLogFile = openResidualLogFile(gInput);

    // main loop//
    int counter = 1;
    for(int it = 0; it < _cgInput.n_max; it++)
    {
        // Initialize conjugate gradient parameters
        PressureFieldSerial gradientCurrent(_grid), gradientPrevious(_grid), zeta(_grid);
        double residualCurrent = 0.0, residualPrevious = 0.0, alpha = 0.0;

        _forwardModel->calculateKappa();
        std::vector<std::complex<double>> &residualArray = _forwardModel->calculateResidual(_chiEstimate, pData);

        // Initialize Regularisation parameters
        double deltaAmplification = _cgInput.dAmplification.start /
                                    (_cgInput.dAmplification.slope * it + 1.0);   // deltaAmplification decreases the step size for increasing iteration step
        RegularisationParameters regularisationCurrent(_grid);
        RegularisationParameters regularisationPrevious(_grid);
        regularisationPrevious.bSquared.Zero();

        // First iteration
        //------------------------------------------------------------------
        zeta = calculateUpdateDirection(residualArray, gradientCurrent, eta);
        alpha = calculateStepSize(zeta, residualArray);   // eq: optimalStepSizeCG

        // Update contrast-function
        _chiEstimate += alpha * zeta;   // eq: contrastUpdate

        // Result + logging
        residualCurrent = errorFunctional(residualArray, pData, eta);   // eq: errorFunc
        logResidualResults(0, it, residualCurrent, counter, residualLogFile);

        // Save variables for next iteration
        gradientPrevious = gradientCurrent;
        residualPrevious = residualCurrent;

        counter++;
        // -----------------------------------------------------------------

        // start the inner loop
        for(int it1 = 1; it1 < _cgInput.iteration1.n; it1++)
        {
            calculateRegularisationParameters(regularisationPrevious, regularisationCurrent, deltaAmplification);

            zeta = calculateUpdateDirectionRegularisation(residualArray, gradientCurrent, gradientPrevious, eta, regularisationCurrent, regularisationPrevious,
                zeta, residualPrevious);   // eq: updateDirectionsCG
            alpha = calculateStepSizeRegularisation(regularisationPrevious, regularisationCurrent, nTotal, residualArray, eta, residualPrevious, zeta);

            // Update contrast-function
            _chiEstimate += alpha * zeta;

            // Result + logging
            residualCurrent = errorFunctional(residualArray, pData, eta);
            logResidualResults(it1, it, residualCurrent, counter, residualLogFile);

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
            _chiEstimate.Gradient(regularisationCurrent.gradientChi);
            calculateRegularisationErrorFunctional(regularisationPrevious, regularisationPrevious);

            regularisationPrevious.deltaSquared = regularisationCurrent.deltaSquared;
            regularisationPrevious.bSquared = regularisationCurrent.bSquared;

            counter++;
            progressBar++;
        }
    }
    residualLogFile.close();

    return _chiEstimate;
}

std::ofstream ConjugateGradientInversion::openResidualLogFile(GenericInput &gInput)
{
    std::ofstream residualLogFile;
    residualLogFile.open(gInput.outputLocation + gInput.runName + "Residual.log", std::ios::out | std::ios::trunc);
    if(!residualLogFile)
    {
        L_(lerror) << "Failed to open the file to store residuals";
        std::exit(EXIT_FAILURE);
    }

    return residualLogFile;
}

PressureFieldSerial ConjugateGradientInversion::calculateUpdateDirection(
    std::vector<std::complex<double>> &residualArray, PressureFieldSerial &gradientCurrent, double eta)
{
    PressureFieldComplexSerial kappaTimesResidual(_grid);   // eq: integrandForDiscreteK of README, KappaTimesResidual is the argument of Re()
    _forwardModel->getUpdateDirectionInformation(residualArray, kappaTimesResidual);
    gradientCurrent = eta * kappaTimesResidual.GetRealPart();

    return gradientCurrent;
}

double ConjugateGradientInversion::calculateStepSize(const PressureFieldSerial &zeta, std::vector<std::complex<double>> &residualArray)
{
    double alphaNumerator = 0.0;
    double alphaDenominator = 0.0;

    int nSignals = _frequencies.nFreq * _sources.nSrc * _receivers.nRecv;

    std::vector<std::complex<double>> kappaTimesZeta(_frequencies.nFreq * _sources.nSrc * _receivers.nRecv);
    _forwardModel->mapDomainToSignal(zeta, kappaTimesZeta);

    for(int i = 0; i < nSignals; i++)
    {
        alphaNumerator += std::real(conj(residualArray[i]) * kappaTimesZeta[i]);
        alphaDenominator += std::real(conj(residualArray[i]) * kappaTimesZeta[i]);
    }

    if(alphaDenominator == 0.0) { throw std::overflow_error("ConjugateGradient: the computation of alpha devides by zero."); }
    double alpha = alphaNumerator / alphaDenominator;

    return alpha;
}

double ConjugateGradientInversion::errorFunctional(std::vector<std::complex<double>> &residualArray, const std::vector<std::complex<double>> &pData, double eta)
{
    residualArray = _forwardModel->calculateResidual(_chiEstimate, pData);
    double residualNormSquared = _forwardModel->calculateResidualNormSq(residualArray);
    double residual = eta * residualNormSquared;

    return residual;
}

void ConjugateGradientInversion::logResidualResults(int it1, int it, double residual, int counter, std::ofstream &residualLogFile)
{
    L_(linfo) << it1 + 1 << "/" << _cgInput.iteration1.n << "\t (" << it + 1 << "/" << _cgInput.n_max << ")\t res: " << std::setprecision(17) << residual;
    residualLogFile << std::setprecision(17) << residual << "," << counter << std::endl;
}

void ConjugateGradientInversion::calculateRegularisationParameters(
    RegularisationParameters &regularisationPrevious, RegularisationParameters &regularisationCurrent, double deltaAmplification)
{
    _chiEstimate.Gradient(regularisationPrevious.gradientChi);
    regularisationPrevious.gradientChiNormSquared = (regularisationPrevious.gradientChi[0] * regularisationPrevious.gradientChi[0]) +
                                                    (regularisationPrevious.gradientChi[1] * regularisationPrevious.gradientChi[1]);

    regularisationCurrent.bSquared = calculateWeightingFactor(regularisationPrevious);   // # eq. 2.22
    regularisationCurrent.b = regularisationCurrent.bSquared;
    regularisationCurrent.b.Sqrt();

    regularisationCurrent.deltaSquared = calculateSteeringFactor(regularisationPrevious, regularisationCurrent, deltaAmplification);   // eq. 2.23
    regularisationCurrent.gradient = calculateRegularisationGradient(regularisationPrevious);
}

PressureFieldSerial ConjugateGradientInversion::calculateWeightingFactor(const RegularisationParameters &regularisationPrevious)
{
    PressureFieldSerial bSquared(_grid);
    bSquared = regularisationPrevious.gradientChiNormSquared + regularisationPrevious.deltaSquared;
    bSquared.Reciprocal();
    bSquared *= (1.0 / (_grid.GetDomainArea()));
    return bSquared;
}

double ConjugateGradientInversion::calculateSteeringFactor(
    const RegularisationParameters &regularisationPrevious, const RegularisationParameters &regularisationCurrent, double deltaAmplification)
{
    PressureFieldSerial bTimesGradientChiXSquared = regularisationCurrent.b * regularisationPrevious.gradientChi[0];
    bTimesGradientChiXSquared.Square();
    PressureFieldSerial bTimesGradientChiZSquared = regularisationCurrent.b * regularisationPrevious.gradientChi[1];
    bTimesGradientChiZSquared.Square();

    double bTimesGradientChiNormSquared = (bTimesGradientChiXSquared + bTimesGradientChiZSquared).Summation();
    double bSquaredSummed = regularisationCurrent.bSquared.Summation();

    return deltaAmplification * 0.5 * bTimesGradientChiNormSquared / bSquaredSummed;
}

PressureFieldSerial ConjugateGradientInversion::calculateRegularisationGradient(const RegularisationParameters &regularisationPrevious)
{
    std::vector<PressureFieldSerial> temporaryGradient(2, PressureFieldSerial(_grid));

    PressureFieldSerial bSquaredTimesGradientChiX = regularisationPrevious.bSquared * regularisationPrevious.gradientChi[0];
    bSquaredTimesGradientChiX.Gradient(temporaryGradient);
    PressureFieldSerial gradientBSquaredTimesGradientChiX = temporaryGradient[0];

    PressureFieldSerial bSquaredTimesGradientChiZ = regularisationPrevious.bSquared * regularisationPrevious.gradientChi[1];
    bSquaredTimesGradientChiZ.Gradient(temporaryGradient);
    PressureFieldSerial gradientBSquaredTimesGradientChiZ = temporaryGradient[1];

    return gradientBSquaredTimesGradientChiX + gradientBSquaredTimesGradientChiZ;
}

PressureFieldSerial ConjugateGradientInversion::calculateUpdateDirectionRegularisation(std::vector<std::complex<double>> &residualArray,
    PressureFieldSerial &gradientCurrent, const PressureFieldSerial &gradientPrevious, const double eta, const RegularisationParameters &regularisationCurrent,
    const RegularisationParameters &regularisationPrevious, const PressureFieldSerial &zeta, double residualPrevious)
{
    PressureFieldComplexSerial kappaTimesResidual(_grid);
    kappaTimesResidual.Zero();
    _forwardModel->getUpdateDirectionInformation(residualArray, kappaTimesResidual);
    gradientCurrent = eta * regularisationPrevious.errorFunctional * kappaTimesResidual.GetRealPart() +
                      residualPrevious * regularisationCurrent.gradient;   // eq: 2.25 of thesis
    double gamma = gradientCurrent.InnerProduct(gradientCurrent - gradientPrevious) /
                   gradientPrevious.InnerProduct(gradientPrevious);   // eq: PolakRibiereDirection README

    return gradientCurrent + gamma * zeta;
}

double ConjugateGradientInversion::calculateStepSizeRegularisation(const RegularisationParameters &regularisationPrevious,
    RegularisationParameters &regularisationCurrent, const int nTotal, const std::vector<std::complex<double>> &residualArray, const double eta,
    const double fDataPrevious, const PressureFieldSerial &zeta)
{
    std::vector<std::complex<double>> kappaTimesZeta(_frequencies.nFreq * _sources.nSrc * _receivers.nRecv);
    _forwardModel->mapDomainToSignal(zeta, kappaTimesZeta);

    double a0 = fDataPrevious;

    double a1 = 0.0;
    double a2 = 0.0;

    for(int i = 0; i < nTotal; i++)
    {
        a1 += -2.0 * eta * std::real(conj(residualArray[i]) * kappaTimesZeta[i]);
        a2 += eta * std::real(conj(kappaTimesZeta[i]) * kappaTimesZeta[i]);
    }

    PressureFieldSerial bGradientChiSquaredXDirection =
        (regularisationCurrent.b * regularisationPrevious.gradientChi[0]) * (regularisationCurrent.b * regularisationPrevious.gradientChi[0]);
    PressureFieldSerial bGradientChiSquaredZDirection =
        (regularisationCurrent.b * regularisationPrevious.gradientChi[1]) * (regularisationCurrent.b * regularisationPrevious.gradientChi[1]);
    double b0 = ((bGradientChiSquaredXDirection.Summation() + bGradientChiSquaredZDirection.Summation()) +
                    regularisationPrevious.deltaSquared * regularisationCurrent.bSquared.Summation()) *
                _grid.GetCellVolume();

    std::vector<PressureFieldSerial> gradientZeta(2, PressureFieldSerial(_grid));
    zeta.Gradient(gradientZeta);

    PressureFieldSerial bGradientZetabGradientChiX =
        (regularisationCurrent.b * gradientZeta[0]) * (regularisationCurrent.b * regularisationPrevious.gradientChi[0]);
    PressureFieldSerial bGradientZetabGradientChiZ =
        (regularisationCurrent.b * gradientZeta[1]) * (regularisationCurrent.b * regularisationPrevious.gradientChi[1]);
    double b1 = 2.0 * (bGradientZetabGradientChiX.Summation() + bGradientZetabGradientChiZ.Summation()) * _grid.GetCellVolume();

    PressureFieldSerial bTimesGradientZetaXdirection = regularisationCurrent.b * gradientZeta[0];
    PressureFieldSerial bTimesGradientZetaZdirection = regularisationCurrent.b * gradientZeta[1];
    bTimesGradientZetaXdirection.Square();
    bTimesGradientZetaZdirection.Square();
    double b2 = (bTimesGradientZetaXdirection.Summation() + bTimesGradientZetaZdirection.Summation()) * _grid.GetCellVolume();

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
    PressureFieldSerial gradientChiNormsquaredCurrent(_grid);
    gradientChiNormsquaredCurrent = (regularisationCurrent.gradientChi[0] * regularisationCurrent.gradientChi[0]) +
                                    (regularisationCurrent.gradientChi[1] * regularisationCurrent.gradientChi[1]);

    PressureFieldSerial integral = (gradientChiNormsquaredCurrent + regularisationPrevious.deltaSquared) /
                                   (regularisationPrevious.gradientChiNormSquared + regularisationPrevious.deltaSquared);

    regularisationPrevious.errorFunctional = (1.0 / (_grid.GetDomainArea())) * integral.Summation() * _grid.GetCellVolume();
}
