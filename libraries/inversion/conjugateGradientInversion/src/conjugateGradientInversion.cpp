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

double ConjugateGradientInversion::findRealRootFromCubic(double a, double b, double c, double d)
{
    // assuming ax^3 + bx^2 +cx + d and assuming only one real root, which is expected in this algorithm
    // uses Cardano's formula
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

double ConjugateGradientInversion::calculateAlpha(PressureFieldSerial& zeta, std::vector<std::complex<double>>& residualArray)
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
    double alpha = alphaNumerator / alphaDenominator;
    return alpha;
}

double ConjugateGradientInversion::calculateAlpha_regression(const std::vector<std::complex<double>>& zetaTemp, std::vector<PressureFieldSerial> &gradientZetaTmp, const int nTotal, double deltasquaredOld, const PressureFieldSerial& b, const PressureFieldSerial& bsquared, const std::vector<std::complex<double>> &resArray, const std::vector<PressureFieldSerial> &gradientChiOld, const double eta, const double fDataOld, const PressureFieldSerial& zeta)
{
    std::array<double, 2> A = {0.0, 0.0};

    for (int i = 0; i < nTotal; i++)
    {
        A[1] += eta * std::real(conj(zetaTemp[i]) * zetaTemp[i]);
        A[0] += double(-2.0) * eta * std::real(conj(resArray[i]) * zetaTemp[i]);
    }

    double A0 = fDataOld;
    zeta.Gradient(gradientZetaTmp);
    PressureFieldSerial tmpVolField = b * gradientZetaTmp[0];
    PressureFieldSerial tmpVolField2 = b * gradientZetaTmp[1];
    tmpVolField.Square();
    tmpVolField2.Square();
    double B2 = (tmpVolField.Summation() + tmpVolField2.Summation()) * _grid.GetCellVolume();

    tmpVolField = (b * gradientZetaTmp[0]) * (b * gradientChiOld[0]);
    tmpVolField2 = (b * gradientZetaTmp[1]) * (b * gradientChiOld[1]);
    double B1 = double(2.0) * (tmpVolField.Summation() + tmpVolField2.Summation()) *
                _grid.GetCellVolume();

    tmpVolField = (b * gradientChiOld[0]) * (b * gradientChiOld[0]);
    tmpVolField2 = (b * gradientChiOld[1]) * (b * gradientChiOld[1]);
    double B0 = ((tmpVolField.Summation() + tmpVolField2.Summation()) + deltasquaredOld *
                                                                            bsquared.Summation()) *
                _grid.GetCellVolume();

    double derA = double(4.0) * A[1] * B2;
    double derB = double(3.0) * (A[1] * B1 + A[0] * B2);
    double derC = double(2.0) * (A[1] * B0 + A[0] * B1 + A0 * B2);
    double derD = A[0] * B0 + A0 * B1;

    return findRealRootFromCubic(derA, derB, derC, derD);
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

void ConjugateGradientInversion::logResidualResults(int it1, int it, double error, int counter, std::ofstream &residualLogFile)
{
    L_(linfo) << it1 + 1 << "/" << _cgInput.iteration1.n << "\t (" << it + 1 << "/" << _cgInput.n_max << ")\t res: " << std::setprecision(17) << error;
    residualLogFile << std::setprecision(17) << error << "," << counter << std::endl;
}

double ConjugateGradientInversion::errorFunctional(std::vector<std::complex<double>> &residualArray, const std::vector<std::complex<double>> &pData, double eta)
{
    residualArray = _forwardModel->calculateResidual(_chiEstimate, pData);
    double residualSquared = _forwardModel->calculateResidualNormSq(residualArray);
    double residual = eta * residualSquared;
    return residual;
}

void ConjugateGradientInversion::calculateSteeringFactor(const std::vector<PressureFieldSerial> &gradientChiPrevious, double deltaAmplification, Regularization &regularizationCurrent)
{
    PressureFieldSerial tmpVolField = regularizationCurrent.b * gradientChiPrevious[0];
    tmpVolField.Square();
    PressureFieldSerial tmpVolField2 = regularizationCurrent.b * gradientChiPrevious[1];
    tmpVolField2.Square();
    tmpVolField += tmpVolField2;
    regularizationCurrent.deltaSquared = deltaAmplification * double(0.5) * tmpVolField.Summation() / regularizationCurrent.bSquared.Summation();
}

void ConjugateGradientInversion::calculateRegularisationGradient(const PressureFieldSerial &bSquaredOld, const std::vector<PressureFieldSerial> &gradientChiOld, std::vector<PressureFieldSerial>&gradientGregTmp, Regularization &regularizationCurrent)
{
    PressureFieldSerial tmpVolFieldx = bSquaredOld * gradientChiOld[0];
    tmpVolFieldx.Gradient(gradientGregTmp);
    tmpVolFieldx = gradientGregTmp[0];
    PressureFieldSerial tmpVolFieldz = bSquaredOld * gradientChiOld[1];
    tmpVolFieldz.Gradient(gradientGregTmp);
    tmpVolFieldz = gradientGregTmp[1];
    regularizationCurrent.gRegularisation = tmpVolFieldx + tmpVolFieldz;
}

void ConjugateGradientInversion::calculateWeightingFactor(const PressureFieldSerial &gradientChiOldNormSquared, const Regularization &regularizationPrevious, Regularization &regularizationCurrent)
{
    regularizationCurrent.bSquared = (gradientChiOldNormSquared + regularizationPrevious.deltaSquared); // eq: errorFuncRegulWeighting
    regularizationCurrent.bSquared.Reciprocal();
    regularizationCurrent.bSquared *= (double(1.0) / (_grid.GetDomainArea()));

    regularizationCurrent.b = regularizationCurrent.bSquared;
    regularizationCurrent.b.Sqrt();
}

PressureFieldSerial ConjugateGradientInversion::Reconstruct(const std::vector<std::complex<double>> &pData, GenericInput gInput)
{
    ProgressBar progressBar(_cgInput.n_max * _cgInput.iteration1.n);

    const int nTotal = _frequencies.nFreq * _sources.nSrc * _receivers.nRecv;
    const double eta = 1.0 / (normSq(pData, nTotal)); //scaling factor eq 2.10 in thesis/ eq: errorFuncSubEtaInv

    double gamma, alpha, residualCurrent = 0.0;

    PressureFieldSerial gradientCurrent(_grid), gradientPrevious(_grid), zeta(_grid);
    PressureFieldComplexSerial kappaTimesResidual(_grid); // eq: integrandForDiscreteK, KappaTimesResidual is the argument of Re()

    _chiEstimate.Zero();

    std::vector<PressureFieldSerial> gradientChiPrevious(2, PressureFieldSerial(_grid));
    std::vector<PressureFieldSerial> gradientGregTmp(2, PressureFieldSerial(_grid));
    std::vector<PressureFieldSerial> gradientZetaTmp(2, PressureFieldSerial(_grid));

    std::ofstream residualLogFile = OpenResidualLogFile(gInput);

    //main loop//
    int counter = 1;
    for (int it = 0; it < _cgInput.n_max; it++)
    {
        double fRegOld = double(1.0);
        double residualPrevious = double(0.0);
        double deltaAmplification = _cgInput.dAmplification.start / (_cgInput.dAmplification.slope * it + double(1.0));

        PressureFieldSerial bSquaredPrevious(_grid);
        bSquaredPrevious.Zero();

        _forwardModel->calculateKappa();

        std::vector<std::complex<double>> &residualArray = _forwardModel->calculateResidual(_chiEstimate, pData);

        // First iteration
        // TODO: Make this separate function - start
        // Calculate update direction
        _forwardModel->getUpdateDirectionInformation(residualArray, kappaTimesResidual);
        gradientCurrent = eta * kappaTimesResidual.GetRealPart();                         // eq: integrandForDiscreteK
        zeta = gradientCurrent;                                                           // eq: updateDirectionsCG

        //Calculate step seize
        alpha = calculateAlpha(zeta, residualArray);                        // eq: optimalStepSizeCG

        // Update contrast-function
        _chiEstimate += alpha * zeta;                                       // eq: contrastUpdate
        // TODO: Make this separate function - end


        // Result + logging
        residualCurrent = errorFunctional(residualArray, pData, eta);              // eq: errorFunc
        logResidualResults(0, it, residualCurrent, counter, residualLogFile);

        // Save variables for next iteration
        gradientPrevious = gradientCurrent;
        residualPrevious = residualCurrent;

        counter++;
        Regularization regularizationCurrent(_grid);
        Regularization regularizationPrevious(_grid);
        regularizationPrevious.deltaSquared = 0.0;


        //start the inner loop
        for (int it1 = 1; it1 < _cgInput.iteration1.n; it1++)
        {
            // Regularisation
            _chiEstimate.Gradient(gradientChiPrevious);
            PressureFieldSerial gradientChiPreviousNormSquared(_grid);
            gradientChiPreviousNormSquared = (gradientChiPrevious[0] * gradientChiPrevious[0]) + (gradientChiPrevious[1] * gradientChiPrevious[1]);

            calculateWeightingFactor(gradientChiPreviousNormSquared, regularizationPrevious, regularizationCurrent); // # eq. 2.22
            calculateSteeringFactor(gradientChiPrevious, deltaAmplification, regularizationCurrent); // eq. 2.23
            calculateRegularisationGradient(bSquaredPrevious, gradientChiPrevious, gradientGregTmp, regularizationCurrent); // eq. 2.24


            // Calculate Direction
            kappaTimesResidual.Zero();
            _forwardModel->getUpdateDirectionInformation(residualArray, kappaTimesResidual);
            gradientCurrent = eta * fRegOld * kappaTimesResidual.GetRealPart() + residualPrevious * regularizationCurrent.gRegularisation;          // eq: 2.25
            gamma = gradientCurrent.InnerProduct(gradientCurrent - gradientPrevious) / gradientPrevious.InnerProduct(gradientPrevious);  // eq: PolakRibiereDirection
            zeta = gradientCurrent + gamma * zeta;                                                                 // eq: updateDirectionsCG

            std::vector<std::complex<double>> kappaTimesZeta(_frequencies.nFreq * _sources.nSrc * _receivers.nRecv);
            _forwardModel->mapDomainToSignal(zeta, kappaTimesZeta);

            // Calculate stepsize
            alpha = calculateAlpha_regression(kappaTimesZeta, gradientZetaTmp, nTotal, regularizationPrevious.deltaSquared, regularizationCurrent.b, regularizationCurrent.bSquared, residualArray, gradientChiPrevious, eta, residualPrevious, zeta);


            //Update
            _chiEstimate += alpha * zeta;                   // eq: contrastUpdate

            // Result + logging
            residualCurrent = errorFunctional(residualArray, pData, eta);       // eq: errorFunc
            logResidualResults(it1, it, residualCurrent, counter, residualLogFile);

            // breakout check
            if ((it1 > 0) && ((residualCurrent < double(_cgInput.iteration1.tolerance)) ||
                                (std::abs(residualPrevious - residualCurrent) < double(_cgInput.iteration1.tolerance)))){
                progressBar.setCounter(_cgInput.iteration1.n + progressBar.getCounter() - (progressBar.getCounter() % _cgInput.iteration1.n));
                break;
            }


            // Save variables for next iteration - Regularisation
            _chiEstimate.Gradient(gradientChiPrevious);
            PressureFieldSerial gradientChiNormsquaredCurrent(_grid);
            gradientChiNormsquaredCurrent = (gradientChiPrevious[0] * gradientChiPrevious[0]) +
                                        (gradientChiPrevious[1] * gradientChiPrevious[1]);

            PressureFieldSerial tmpVolField = (gradientChiNormsquaredCurrent + regularizationPrevious.deltaSquared) / (gradientChiPreviousNormSquared + regularizationPrevious.deltaSquared);
            fRegOld = (double(1.0) / (_grid.GetDomainArea())) *
                        tmpVolField.Summation() * _grid.GetCellVolume();


            regularizationPrevious.deltaSquared = regularizationCurrent.deltaSquared;
            bSquaredPrevious = regularizationCurrent.bSquared;

            // Save 'normal' variables for next iteration
            gradientPrevious = gradientCurrent;
            residualPrevious = residualCurrent;


            counter++;
            progressBar++;
        }
    }

    residualLogFile.close();


    PressureFieldSerial result(_grid);
    _chiEstimate.CopyTo(result);
    return result;
}
