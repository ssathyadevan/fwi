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
    : _forwardModel(), _cgInput(invInput), _grid(forwardModel->getGrid()), _sources(forwardModel->getSrc()), _receivers(forwardModel->getRecv()), _frequencies(forwardModel->getFreq())
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
    double alphaNumerator = 0.0;
    double alphaDenominator = 0.0;
    int nSignals = _frequencies.nFreq * _sources.nSrc * _receivers.nRecv;

    std::vector<std::complex<double>> KappaTimesZeta(_frequencies.nFreq * _sources.nSrc * _receivers.nRecv);
    _forwardModel->mapDomainToSignal(zeta, KappaTimesZeta);

    for (int i = 0; i < nSignals; i++)
    {
        alphaNumerator += std::real(conj(residualArray[i]) * KappaTimesZeta[i]);
        alphaDenominator += std::real(conj(residualArray[i]) * KappaTimesZeta[i]);
    }

    return alphaNumerator / alphaDenominator;
}

double ConjugateGradientInversion::calculateAlpha_regression(const std::vector<std::complex<double>>& zetaTemp, PressureFieldSerial **gradientZetaTmp, const int nTotal, double deltasquaredOld, const PressureFieldSerial& b, const PressureFieldSerial& bsquared, const std::vector<std::complex<double>> &resArray, PressureFieldSerial **gradientChiOld, const double eta, const double fDataOld, const PressureFieldSerial& zeta)
{
    std::array<double, 2> A = {0.0, 0.0};

    for (int i = 0; i < nTotal; i++)
    {
        A[1] += eta * std::real(conj(zetaTemp[i]) * zetaTemp[i]);
        A[0] += double(-2.0) * eta * std::real(conj(resArray[i]) * zetaTemp[i]);
    }

    double A0 = fDataOld;
    zeta.Gradient(gradientZetaTmp);
    PressureFieldSerial tmpVolField = b * *gradientZetaTmp[0];
    PressureFieldSerial tmpVolField2 = b * *gradientZetaTmp[1];
    tmpVolField.Square();
    tmpVolField2.Square();
    double B2 = (tmpVolField.Summation() + tmpVolField2.Summation()) * _grid.GetCellVolume();

    tmpVolField = (b * *gradientZetaTmp[0]) * (b * *gradientChiOld[0]);
    tmpVolField2 = (b * *gradientZetaTmp[1]) * (b * *gradientChiOld[1]);
    double B1 = double(2.0) * (tmpVolField.Summation() + tmpVolField2.Summation()) *
                _grid.GetCellVolume();

    tmpVolField = (b * *gradientChiOld[0]) * (b * *gradientChiOld[0]);
    tmpVolField2 = (b * *gradientChiOld[1]) * (b * *gradientChiOld[1]);
    double B0 = ((tmpVolField.Summation() + tmpVolField2.Summation()) + deltasquaredOld *
                                                                            bsquared.Summation()) *
                _grid.GetCellVolume();

    double derA = double(4.0) * A[1] * B2;
    double derB = double(3.0) * (A[1] * B1 + A[0] * B2);
    double derC = double(2.0) * (A[1] * B0 + A[0] * B1 + A0 * B2);
    double derD = A[0] * B0 + A0 * B1;

    return findRealRootFromCubic(derA, derB, derC, derD);
}

std::ofstream ConjugateGradientInversion::OpenResidualLogFile(GenericInput gInput)
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

PressureFieldSerial ConjugateGradientInversion::Reconstruct(const std::vector<std::complex<double>> &pData, GenericInput gInput)
{
    ProgressBar bar(_cgInput.n_max * _cgInput.iteration1.n);

    const int nTotal = _frequencies.nFreq * _sources.nSrc * _receivers.nRecv;
    const double eta = 1.0 / (normSq(pData, nTotal)); //scaling factor eq 2.10 in thesis

    double gamma, alpha, residualSquared, error = 0;

    PressureFieldSerial chiEstimate(_grid), g(_grid), gOld(_grid), zeta(_grid);

    PressureFieldComplexSerial KappaTimesResidual(_grid); // eq: integrandForDiscreteK, tmp is the argument of Re()

    chiEstimate.Zero();

    PressureFieldSerial **gradientChiOld = new PressureFieldSerial *[2];
    PressureFieldSerial **gradientGregTmp = new PressureFieldSerial *[2];
    PressureFieldSerial **gradientZetaTmp = new PressureFieldSerial *[2];

    for (int i = 0; i < 2; i++)
    {
        gradientChiOld[i] = new PressureFieldSerial(_grid);
        gradientGregTmp[i] = new PressureFieldSerial(_grid);
        gradientZetaTmp[i] = new PressureFieldSerial(_grid);
    }

    std::ofstream residualLogFile = OpenResidualLogFile(gInput);

    //main loop//
    int counter = 1;
    for (int it = 0; it < _cgInput.n_max; it++)
    {

        double deltasquaredOld = double(0.0);
        double fRegOld = double(1.0);
        double errorOld = double(0.0);
        double deltaAmplification = _cgInput.dAmplification.start / (_cgInput.dAmplification.slope * it + double(1.0));

        PressureFieldSerial bsquaredOld(_grid);
        bsquaredOld.Zero();

        _forwardModel->calculateKappa();

        std::vector<std::complex<double>> &residualArray = _forwardModel->calculateResidual(chiEstimate, pData);

        //start the inner loop
        for (int it1 = 0; it1 < _cgInput.iteration1.n; it1++)
        {
            bool isFirstIteration = (it1 == 0);
            if (isFirstIteration)
            {
                _forwardModel->getUpdateDirectionInformation(residualArray, KappaTimesResidual);

                g = eta * KappaTimesResidual.GetRealPart(); //eq: gradientRunc
                zeta = g;
                alpha = calculateAlpha(zeta, residualArray);

                chiEstimate += alpha * zeta;

                // eq: errorFunc
                residualArray = _forwardModel->calculateResidual(chiEstimate, pData);
                residualSquared = _forwardModel->calculateResidualNormSq(residualArray);
                error = eta * residualSquared;

                gOld = g;
                errorOld = error;

                // store the residual value in the residual log
                L_(linfo) << it1 + 1 << "/" << _cgInput.iteration1.n << "\t (" << it + 1 << "/" << _cgInput.n_max << ")\t res: " << std::setprecision(17) << error;
                residualLogFile << std::setprecision(17) << error << "," << counter << std::endl;

                counter++;
            }
            else
            {
                chiEstimate.Gradient(gradientChiOld);
                PressureFieldSerial gradientChiOldNormsquared(_grid);

                gradientChiOldNormsquared = (*gradientChiOld[0] * *gradientChiOld[0]) + (*gradientChiOld[1] * *gradientChiOld[1]);

                PressureFieldSerial bsquared = (gradientChiOldNormsquared + deltasquaredOld); // eq: errorFuncRegulWeighting
                bsquared.Reciprocal();
                bsquared *= (double(1.0) / (_grid.GetDomainArea())); // # eq. 2.22
                PressureFieldSerial b = bsquared;
                b.Sqrt();

                PressureFieldSerial tmpVolField = b * *gradientChiOld[0];
                tmpVolField.Square();
                PressureFieldSerial tmpVolField2 = b * *gradientChiOld[1];
                tmpVolField2.Square();
                tmpVolField += tmpVolField2;
                double deltasquared = deltaAmplification * double(0.5) * tmpVolField.Summation() / bsquared.Summation(); // # eq. 2.23

                tmpVolField = bsquaredOld * *gradientChiOld[0];
                tmpVolField.Gradient(gradientGregTmp);
                tmpVolField = *gradientGregTmp[0];
                tmpVolField2 = bsquaredOld * *gradientChiOld[1];
                tmpVolField2.Gradient(gradientGregTmp);
                tmpVolField2 = *gradientGregTmp[1];

                PressureFieldSerial gReg = tmpVolField + tmpVolField2; //# eq. 2.24
                KappaTimesResidual.Zero();

                _forwardModel->getUpdateDirectionInformation(residualArray, KappaTimesResidual);

                g = eta * fRegOld * KappaTimesResidual.GetRealPart() + errorOld * gReg; // # eq: integrandForDiscreteK

                gamma = g.InnerProduct(g - gOld) / gOld.InnerProduct(gOld); // # eq: PolakRibiereDirection
                zeta = g + gamma * zeta;

                std::vector<std::complex<double>> zetaTemp(_frequencies.nFreq * _sources.nSrc * _receivers.nRecv);
                _forwardModel->mapDomainToSignal(zeta, zetaTemp);
                alpha = calculateAlpha_regression(zetaTemp, gradientZetaTmp, nTotal, deltasquaredOld, b, bsquared, residualArray, gradientChiOld, eta, errorOld, zeta);

                chiEstimate += alpha * zeta;

                residualArray = _forwardModel->calculateResidual(chiEstimate, pData);
                residualSquared = _forwardModel->calculateResidualNormSq(residualArray);
                error = eta * residualSquared;

                L_(linfo) << it1 + 1 << "/" << _cgInput.iteration1.n << "\t (" << it + 1 << "/" << _cgInput.n_max << ")\t res: "
                            << std::setprecision(17) << error;

                residualLogFile << std::setprecision(17) << error << "," << counter << std::endl; // store the residual value in the residual log
                counter++;


                //breakout check
                if ((it1 > 0) && ((error < double(_cgInput.iteration1.tolerance)) ||
                                    (std::abs(errorOld - error) < double(_cgInput.iteration1.tolerance)))){
                    bar.setCounter(_cgInput.iteration1.n + bar.getCounter() - (bar.getCounter() % _cgInput.iteration1.n));
                    break;                  
                }
                    
                //                    L_(linfo) << "Relative Tol: " << res/std::abs(vecResFirstIter[0]) << std::endl;
                //                    if ( (it1 > 0) && ( res/std::abs(vecResFirstIter[0]) < 0.15 )  )
                //                        break;

                chiEstimate.Gradient(gradientChiOld);
                PressureFieldSerial gradientChiNormsquared(_grid);
                gradientChiNormsquared = (*gradientChiOld[0] * *gradientChiOld[0]) +
                                            (*gradientChiOld[1] * *gradientChiOld[1]);

                tmpVolField = (gradientChiNormsquared + deltasquaredOld) / (gradientChiOldNormsquared + deltasquaredOld);
                fRegOld = (double(1.0) / (_grid.GetDomainArea())) *
                            tmpVolField.Summation() * _grid.GetCellVolume();
                errorOld = error;
                deltasquaredOld = deltasquared;
                gOld = g;
                bsquaredOld = bsquared;
            }
            bar++;
        }
    }

    // Clean the mess
    residualLogFile.close();

    for (int i = 0; i < 2; i++)
    {
        delete gradientChiOld[i];
        delete gradientGregTmp[i];
        delete gradientZetaTmp[i];
    }
    delete[] gradientChiOld;
    delete[] gradientGregTmp;
    delete[] gradientZetaTmp;

    PressureFieldSerial result(_grid);
    chiEstimate.CopyTo(result);
    return result;
}
