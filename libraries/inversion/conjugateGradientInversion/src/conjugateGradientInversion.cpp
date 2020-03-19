#include <memory>
#include "conjugateGradientInversion.h"
#include "progressBar.h"
#include "log.h"
#include <iostream>

#define TAG_COMMAND     0
    #define COMMAND_GETUPDATEDIRECTION  1
    #define COMMAND_EXIT                -99
#define TAG_SIZE        1 
#define TAG_RESARRAY    2 
#define TAG_RESULT      3


ConjugateGradientInversion::ConjugateGradientInversion(ForwardModelInterface *forwardModel, const ConjugateGradientInversionInput &invInput)
    : _forwardModel(), _cgInput(invInput), _grid(forwardModel->getGrid())
    , _sources(forwardModel->getSrc()), _receivers(forwardModel->getRecv())
    , _frequencies(forwardModel->getFreq()), _chiEstimate(_grid)
{
    _forwardModel = forwardModel;
}


PressureFieldSerial ConjugateGradientInversion::Reconstruct(const std::vector<std::complex<double>> &pData, GenericInput gInput)
{
    ProgressBar progressBar(_cgInput.n_max * _cgInput.iteration1.n);

    const int nTotal = _frequencies.nFreq * _sources.nSrc * _receivers.nRecv;
    const double eta = 1.0 / (normSq(pData, nTotal)); // Scaling factor
    _chiEstimate.Zero();

    std::ofstream residualLogFile = OpenResidualLogFile(gInput);

    //main loop//
    int counter = 1;
    for (int it = 0; it < _cgInput.n_max; it++)
    {
        // Initialize conjugate gradient parameters
        PressureFieldSerial gradientCurrent(_grid), gradientPrevious(_grid), zeta(_grid);
        double residualPrevious = double(0.0);
        double alpha, residualCurrent = 0.0;
        _forwardModel->calculateKappa();
        std::vector<std::complex<double>> &residualArray = _forwardModel->calculateResidual(_chiEstimate, pData);

        // Initialize Regularisation parameters
        double deltaAmplification = _cgInput.dAmplification.start / (_cgInput.dAmplification.slope * it + double(1.0));
        RegularisationParameters regularisationCurrent(_grid);
        RegularisationParameters regularisationPrevious(_grid);
        regularisationPrevious.deltaSquared = 0.0;
        regularisationPrevious.bSquared.Zero();
        regularisationPrevious.errorFunctional = 1.0;

        // First iteration
        //------------------------------------------------------------------
        zeta = calculateUpdateDirection(residualArray, gradientCurrent, eta);
        alpha = calculateStepSize(zeta, residualArray);                        // eq: optimalStepSizeCG

        // Update contrast-function
        _chiEstimate += alpha * zeta;                                       // eq: contrastUpdate

        // Result + logging
        residualCurrent = errorFunctional(residualArray, pData, eta);              // eq: errorFunc
        logResidualResults(0, it, residualCurrent, counter, residualLogFile);

        // Save variables for next iteration
        gradientPrevious = gradientCurrent;
        residualPrevious = residualCurrent;

        counter++;
        // -----------------------------------------------------------------

        //start the inner loop
        for (int it1 = 1; it1 < _cgInput.iteration1.n; it1++)
        {
            calculateRegularisationParameters(regularisationPrevious, regularisationCurrent, deltaAmplification);

            zeta = calculateUpdateDirection_regularisation(residualArray, gradientCurrent, gradientPrevious, eta, regularisationCurrent, regularisationPrevious, zeta, residualPrevious);                                                                 // eq: updateDirectionsCG
            alpha = calculateStepSize_regularisation(regularisationPrevious, regularisationCurrent, nTotal, residualArray, eta, residualPrevious, zeta);

            // Update contrast-function
            _chiEstimate += alpha * zeta;

            // Result + logging
            residualCurrent = errorFunctional(residualArray, pData, eta);
            logResidualResults(it1, it, residualCurrent, counter, residualLogFile);

            // breakout check
            if ((it1 > 0) && ((residualCurrent < double(_cgInput.iteration1.tolerance)) ||
                                (std::abs(residualPrevious - residualCurrent) < double(_cgInput.iteration1.tolerance)))){
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

std::ofstream ConjugateGradientInversion::OpenResidualLogFile(GenericInput &gInput)
{
    std::ofstream residualLogFile;
    residualLogFile.open(gInput.outputLocation + gInput.runName + "Residual.log", std::ios::out | std::ios::trunc);
    if (!residualLogFile)
    {
        L_(lerror) << "Failed to open the file to store residuals" ;
        std::exit(EXIT_FAILURE);
    }

    return residualLogFile;
}

PressureFieldSerial ConjugateGradientInversion::calculateUpdateDirection(std::vector<std::complex<double>> &residualArray, PressureFieldSerial &gradientCurrent, const double eta)
{
    PressureFieldComplexSerial kappaTimesResidual(_grid); // eq: integrandForDiscreteK of README, KappaTimesResidual is the argument of Re()
    _forwardModel->getUpdateDirectionInformation(residualArray, kappaTimesResidual);
    gradientCurrent = eta * kappaTimesResidual.GetRealPart();

    return gradientCurrent;
}

double ConjugateGradientInversion::calculateStepSize(PressureFieldSerial& zeta, std::vector<std::complex<double>>& residualArray)
{
    double alphaNumerator = double(0.0);
    double alphaDenominator = double(0.0);

    int nSignals = _frequencies.nFreq * _sources.nSrc * _receivers.nRecv;

    std::vector<std::complex<double>> kappaTimesZeta(_frequencies.nFreq * _sources.nSrc * _receivers.nRecv);
    _forwardModel->mapDomainToSignal(zeta, kappaTimesZeta);

    for (int i = 0; i < nSignals; i++)
    {
        alphaNumerator += std::real(conj(residualArray[i]) * kappaTimesZeta[i]);
        alphaDenominator += std::real(conj(residualArray[i]) * kappaTimesZeta[i]);
    }

    if (alphaDenominator == 0.0) {throw std::overflow_error("ConjugateGradient: the computation of alpha devides by zero.");}
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

void ConjugateGradientInversion::calculateRegularisationParameters(RegularisationParameters &regularisationPrevious, RegularisationParameters &regularisationCurrent, double deltaAmplification)
{
    _chiEstimate.Gradient(regularisationPrevious.gradientChi);
    regularisationPrevious.gradientChiNormSquared = (regularisationPrevious.gradientChi[0] * regularisationPrevious.gradientChi[0]) + (regularisationPrevious.gradientChi[1] * regularisationPrevious.gradientChi[1]);

    calculateWeightingFactor(regularisationPrevious, regularisationCurrent); // # eq. 2.22
    calculateSteeringFactor(regularisationPrevious, regularisationCurrent, deltaAmplification); // eq. 2.23
    calculateRegularisationGradient(regularisationPrevious, regularisationCurrent);
}


void ConjugateGradientInversion::calculateWeightingFactor(const RegularisationParameters &regularisationPrevious, RegularisationParameters &regularisationCurrent)
{
    regularisationCurrent.bSquared = (regularisationPrevious.gradientChiNormSquared + regularisationPrevious.deltaSquared);
    regularisationCurrent.bSquared.Reciprocal();
    regularisationCurrent.bSquared *= (double(1.0) / (_grid.GetDomainArea()));

    regularisationCurrent.b = regularisationCurrent.bSquared;
    regularisationCurrent.b.Sqrt();
}

void ConjugateGradientInversion::calculateSteeringFactor(const RegularisationParameters &regularisationPrevious, RegularisationParameters &regularisationCurrent,  double deltaAmplification)
{
    PressureFieldSerial tmpVolField = regularisationCurrent.b * regularisationPrevious.gradientChi[0];
    tmpVolField.Square();
    PressureFieldSerial tmpVolField2 = regularisationCurrent.b * regularisationPrevious.gradientChi[1];
    tmpVolField2.Square();
    tmpVolField += tmpVolField2;
    regularisationCurrent.deltaSquared = deltaAmplification * double(0.5) * tmpVolField.Summation() / regularisationCurrent.bSquared.Summation();
}

void ConjugateGradientInversion::calculateRegularisationGradient(const RegularisationParameters &regularisationPrevious, RegularisationParameters &regularisationCurrent)
{
    std::vector<PressureFieldSerial> gradientGregTmp(2, PressureFieldSerial(_grid));
    PressureFieldSerial tmpVolFieldx = regularisationPrevious.bSquared * regularisationPrevious.gradientChi[0];
    tmpVolFieldx.Gradient(gradientGregTmp);
    tmpVolFieldx = gradientGregTmp[0];
    PressureFieldSerial tmpVolFieldz = regularisationPrevious.bSquared * regularisationPrevious.gradientChi[1];
    tmpVolFieldz.Gradient(gradientGregTmp);
    tmpVolFieldz = gradientGregTmp[1];
    regularisationCurrent.gRegularisation = tmpVolFieldx + tmpVolFieldz;
}

PressureFieldSerial ConjugateGradientInversion::calculateUpdateDirection_regularisation(std::vector<std::complex<double>> &residualArray, PressureFieldSerial &gradientCurrent, const PressureFieldSerial &gradientPrevious, const double eta, const RegularisationParameters &regularisationCurrent, const RegularisationParameters &regularisationPrevious, PressureFieldSerial &zeta, double residualPrevious)
{
    PressureFieldComplexSerial kappaTimesResidual(_grid);
    kappaTimesResidual.Zero();
    _forwardModel->getUpdateDirectionInformation(residualArray, kappaTimesResidual);
    gradientCurrent = eta * regularisationPrevious.errorFunctional * kappaTimesResidual.GetRealPart() + residualPrevious * regularisationCurrent.gRegularisation;          // eq: 2.25 of thesis
    double gamma = gradientCurrent.InnerProduct(gradientCurrent - gradientPrevious) / gradientPrevious.InnerProduct(gradientPrevious);  // eq: PolakRibiereDirection README
    zeta = gradientCurrent + gamma * zeta;

    return zeta;
}

double ConjugateGradientInversion::calculateStepSize_regularisation(const RegularisationParameters &regularisationPrevious, RegularisationParameters &regularisationCurrent, const int nTotal, const std::vector<std::complex<double>> &residualArray, const double eta, const double fDataPrevious, const PressureFieldSerial& zeta)
{
    std::vector<std::complex<double>> kappaTimesZeta(_frequencies.nFreq * _sources.nSrc * _receivers.nRecv);
    _forwardModel->mapDomainToSignal(zeta, kappaTimesZeta);

    double A1 = 0;
    double A2 = 0;

    for (int i = 0; i < nTotal; i++)
    {
        A2 += eta * std::real(conj(kappaTimesZeta[i]) * kappaTimesZeta[i]);
        A1 += double(-2.0) * eta * std::real(conj(residualArray[i]) * kappaTimesZeta[i]);
    }

    double A0 = fDataPrevious;

    std::vector<PressureFieldSerial> gradientZeta(2, PressureFieldSerial(_grid));

    zeta.Gradient(gradientZeta);
    PressureFieldSerial bTimesGradientZetaXdirection = regularisationCurrent.b * gradientZeta[0];
    PressureFieldSerial bTimesGradientZetaZdirection = regularisationCurrent.b * gradientZeta[1];
    bTimesGradientZetaXdirection.Square();
    bTimesGradientZetaZdirection.Square();
    double B2 = (bTimesGradientZetaXdirection.Summation() + bTimesGradientZetaZdirection.Summation()) * _grid.GetCellVolume();

    PressureFieldSerial bGradientZetabGradientChiXDirection = (regularisationCurrent.b * gradientZeta[0]) * (regularisationCurrent.b * regularisationPrevious.gradientChi[0]);
    PressureFieldSerial bGradientZetabGradientChiZDirection = (regularisationCurrent.b * gradientZeta[1]) * (regularisationCurrent.b * regularisationPrevious.gradientChi[1]);
    double B1 = double(2.0) * (bGradientZetabGradientChiXDirection.Summation() + bGradientZetabGradientChiZDirection.Summation()) * _grid.GetCellVolume();

    PressureFieldSerial bGradientChiSquaredXDirection = (regularisationCurrent.b * regularisationPrevious.gradientChi[0]) * (regularisationCurrent.b * regularisationPrevious.gradientChi[0]);
    PressureFieldSerial bGradientChiSquaredZDirection = (regularisationCurrent.b * regularisationPrevious.gradientChi[1]) * (regularisationCurrent.b * regularisationPrevious.gradientChi[1]);
    double B0 = ((bGradientChiSquaredXDirection.Summation() + bGradientChiSquaredZDirection.Summation())
                 + regularisationPrevious.deltaSquared * regularisationCurrent.bSquared.Summation())
                 * _grid.GetCellVolume();

    double derA = double(4.0) * A2 * B2;
    double derB = double(3.0) * (A2 * B1 + A1 * B2);
    double derC = double(2.0) * (A2 * B0 + A1 * B1 + A0 * B2);
    double derD = A1 * B0 + A0 * B1;

    return findRealRootFromCubic(derA, derB, derC, derD);
}

double ConjugateGradientInversion::findRealRootFromCubic(double a, double b, double c, double d)
{

    double f = ((double(3.0) * c / a) - (std::pow(b, 2) / std::pow(a, 2))) / double(3.0);
    double g = ((double(2.0) * std::pow(b, 3) / std::pow(a, 3)) -
                (double(9.0) * b * c / std::pow(a, 2)) +
                (double(27.0) * d / a)) /
               double(27.0);
    double h = (std::pow(g, 2) / double(4.0)) + (std::pow(f, 3) / double(27.0));
    double r = -(g / double(2.0)) + std::sqrt(h);
    double s = std::cbrt(r);
    double t = -(g / double(2.0)) - std::sqrt(h);
    double u = std::cbrt(t);

    double realRoot = s + u - (b / (double(3.0) * a));

    return realRoot;
}

void ConjugateGradientInversion::calculateRegularisationErrorFunctional(RegularisationParameters &regularisationPrevious, RegularisationParameters &regularisationCurrent)
{
    PressureFieldSerial gradientChiNormsquaredCurrent(_grid);
    gradientChiNormsquaredCurrent = (regularisationCurrent.gradientChi[0] * regularisationCurrent.gradientChi[0])
                                    + (regularisationCurrent.gradientChi[1] * regularisationCurrent.gradientChi[1]);

    PressureFieldSerial integral = (gradientChiNormsquaredCurrent + regularisationPrevious.deltaSquared)
                                    / (regularisationPrevious.gradientChiNormSquared + regularisationPrevious.deltaSquared);

    regularisationPrevious.errorFunctional = (double(1.0) / (_grid.GetDomainArea())) * integral.Summation() * _grid.GetCellVolume();
}


