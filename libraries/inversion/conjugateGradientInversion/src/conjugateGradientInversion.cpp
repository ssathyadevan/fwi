#include <memory>
#include "conjugateGradientInversion.h"
#include "progressBar.h"

ConjugateGradientInversion::ConjugateGradientInversion(ForwardModelInterface *forwardModel, const GenericInput &gInput)
    : _forwardModel(), _cgInput(), _grid(forwardModel->getGrid()), _src(forwardModel->getSrc()), _recv(forwardModel->getRecv()), _freq(forwardModel->getFreq())
{
    ConjugateGradientInversionInputCardReader ConjugateGradientInversionReader(gInput.caseFolder);
    _forwardModel = forwardModel;
    _cgInput = ConjugateGradientInversionReader.getInput();
}

double ConjugateGradientInversion::findRealRootFromCubic(double a, double b, double c, double d)
{
    // assuming ax^3 + bx^2 +cx + d and assuming only one real root, which is expected in this algorithm
    // uses Cardano's formula
    double f = ((3.0 * c / a) - (std::pow(b, 2) / std::pow(a, 2))) / 3.0;
    double g = ((2.0 * std::pow(b, 3) / std::pow(a, 3)) -
                (9.0 * b * c / std::pow(a, 2)) +
                (27.0 * d / a)) /
               27.0;
    double h = (std::pow(g, 2) / 4.0) + (std::pow(f, 3) / 27.0);
    double r = -(g / 2.0) + std::sqrt(h);
    double s = std::cbrt(r);
    double t = -(g / 2.0) - std::sqrt(h);
    double u = std::cbrt(t);

    double realroot = s + u - (b / (3.0 * a));
    return realroot;
}

double ConjugateGradientInversion::calculateAlpha(std::vector<std::complex<double>> &residuals, PressureFieldSerial &zeta)
{
    int nTotal = _freq.nFreq * _src.nSrc * _recv.nRecv;

    double alpha_numerator = 0.0;
    double alpha_denominator = 0.0;

    std::vector<std::complex<double>> zetaTemp(nTotal);
    _forwardModel->mapDomainToSignal(zeta, zetaTemp);

    for (int i = 0; i < nTotal; i++)
    {
        alpha_numerator += std::real(conj(residuals[i]) * zetaTemp[i]);
        alpha_denominator += std::real(conj(zetaTemp[i]) * zetaTemp[i]);
    }

    return alpha_numerator / alpha_denominator;
}

PressureFieldSerial ConjugateGradientInversion::Reconstruct(const std::vector<std::complex<double>> &pData, GenericInput gInput)
{
    ProgressBar bar(_cgInput.n_max * _cgInput.iteration1.n);

    ProgressBar bar(_cgInput.n_max * _cgInput.iteration1.n);
    const int nTotal = _freq.nFreq * _src.nSrc * _recv.nRecv;

    double eta = 1.0 / (normSq(pData, nTotal));
    double gamma, alpha, resSq, res = 0;

    double deltasquaredOld;
    double fRegOld;
    double fDataOld;
    double deltaAmplification;

    double derA;
    double derB;
    double derC;
    double derD;

    double A0;
    double A1;
    double A2;
    double B0;
    double B1;
    double B2;

    double deltasquared;

    std::vector<std::complex<double>> zetaTemp(_freq.nFreq * _src.nSrc * _recv.nRecv);
    std::vector<std::complex<double>> residuals;
    std::vector<std::complex<double>> vecResFirstIter;

    PressureFieldSerial chiEst(_grid);
    PressureFieldSerial g(_grid);
    PressureFieldSerial gOld(_grid);
    PressureFieldSerial zeta(_grid);
    PressureFieldComplexSerial tmp(_grid);
    PressureFieldSerial gReg(_grid);

    PressureFieldSerial gradientChiNormsquared(_grid);
    PressureFieldSerial bsquared(_grid);
    PressureFieldSerial b(_grid);
    PressureFieldSerial tmpVolField(_grid);

    PressureFieldSerial **gradientChiOld = new PressureFieldSerial *[2];
    PressureFieldSerial **gradientGregTmp = new PressureFieldSerial *[2];
    PressureFieldSerial **gradientZetaTmp = new PressureFieldSerial *[2];

    chiEst.Zero();

    for (int i = 0; i < 2; i++)
    {
        gradientChiOld[i] = new PressureFieldSerial(_grid);
        gradientGregTmp[i] = new PressureFieldSerial(_grid);
        gradientZetaTmp[i] = new PressureFieldSerial(_grid);
    }

    // open the file to store the residual log
    std::ofstream file;
    file.open(gInput.outputLocation + gInput.runName + "Residual.log", std::ios::out | std::ios::trunc);
    if (!file)
    {
        std::cout << "Failed to open the file to store residuals" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    int counter = 1;

    for (int it = 0; it < _cgInput.n_max; it++)
    {
        vecResFirstIter.clear();

        deltasquaredOld = 0.0;
        fRegOld = 1.0;
        fDataOld = 0.0;
        deltaAmplification = _cgInput.dAmplification.start / (_cgInput.dAmplification.slope * it + 1.0);

        PressureFieldSerial bsquaredOld(_grid);
        bsquaredOld.Zero();

        _forwardModel->calculateKappa();

        residuals = _forwardModel->calculateResidual(chiEst, pData);

        //start the inner loop
        for (int it1 = 0; it1 < _cgInput.iteration1.n; it1++)
        {
            if (it1 == 0)
            {

                _forwardModel->getUpdateDirectionInformation(residuals, tmp);

                g = eta * tmp.GetRealPart();
                zeta = g;

                alpha = calculateAlpha(residuals, zeta);
                chiEst += alpha * zeta;
                gOld = g;

                residuals = _forwardModel->calculateResidual(chiEst, pData);
                resSq = _forwardModel->calculateResidualNormSq(residuals);

                res = eta * resSq;

                std::cout << it1 + 1 << "/" << _cgInput.iteration1.n << "\t (" << it + 1 << "/" << _cgInput.n_max << ")\t res: " << std::setprecision(17) << res << std::endl;

                file << std::setprecision(17) << res << "," << counter << std::endl;
                counter++;

                fDataOld = res;
                vecResFirstIter.push_back(res);
            }
            else
            {
                chiEst.Gradient(gradientChiOld);
                PressureFieldSerial gradientChiOldNormsquared(_grid);

                gradientChiOldNormsquared = (*gradientChiOld[0] * *gradientChiOld[0]) + (*gradientChiOld[1] * *gradientChiOld[1]);

                bsquared = (gradientChiOldNormsquared + deltasquaredOld); // eq: errorFuncRegulWeighting
                bsquared.Reciprocal();
                bsquared *= (1.0 / (_grid.GetDomainArea())); // # eq. 2.22
                b = bsquared;
                b.Sqrt();

                tmpVolField = b * *gradientChiOld[0];
                tmpVolField.Square();
                PressureFieldSerial tmpVolField2 = b * *gradientChiOld[1];
                tmpVolField2.Square();
                tmpVolField += tmpVolField2;
                deltasquared = deltaAmplification * 0.5 * tmpVolField.Summation() / bsquared.Summation(); // # eq. 2.23

                tmpVolField = bsquaredOld * *gradientChiOld[0];
                tmpVolField.Gradient(gradientGregTmp);
                tmpVolField = *gradientGregTmp[0];
                tmpVolField2 = bsquaredOld * *gradientChiOld[1];
                tmpVolField2.Gradient(gradientGregTmp);
                tmpVolField2 = *gradientGregTmp[1];

                gReg = tmpVolField + tmpVolField2; //# eq. 2.24
                tmp.Zero();

                _forwardModel->getUpdateDirectionInformation(residuals, tmp);

                g = eta * fRegOld * tmp.GetRealPart() + fDataOld * gReg; // # eq: integrandForDiscreteK

                gamma = g.InnerProduct(g - gOld) / gOld.InnerProduct(gOld); // # eq: PolakRibiereDirection
                zeta = g + gamma * zeta;

                A1 = 0.0;
                A2 = 0.0;

                zetaTemp.clear();
                _forwardModel->mapDomainToSignal(zeta, zetaTemp);

                for (int i = 0; i < nTotal; i++)
                {
                    A2 += eta * std::real(conj(zetaTemp[i]) * zetaTemp[i]);
                    A1 += -2.0 * eta * std::real(conj(residuals[i]) * zetaTemp[i]);
                }

                A0 = fDataOld;
                zeta.Gradient(gradientZetaTmp);
                tmpVolField = b * *gradientZetaTmp[0];
                tmpVolField2 = b * *gradientZetaTmp[1];
                tmpVolField.Square();
                tmpVolField2.Square();
                B2 = (tmpVolField.Summation() + tmpVolField2.Summation()) * _grid.GetCellVolume();

                tmpVolField = (b * *gradientZetaTmp[0]) * (b * *gradientChiOld[0]);
                tmpVolField2 = (b * *gradientZetaTmp[1]) * (b * *gradientChiOld[1]);
                B1 = 2.0 * (tmpVolField.Summation() + tmpVolField2.Summation()) *
                     _grid.GetCellVolume();

                tmpVolField = (b * *gradientChiOld[0]) * (b * *gradientChiOld[0]);
                tmpVolField2 = (b * *gradientChiOld[1]) * (b * *gradientChiOld[1]);
                B0 = ((tmpVolField.Summation() + tmpVolField2.Summation()) + deltasquaredOld *
                                                                                 bsquared.Summation()) *
                     _grid.GetCellVolume();

                derA = 4.0 * A2 * B2;
                derB = 3.0 * (A2 * B1 + A1 * B2);
                derC = 2.0 * (A2 * B0 + A1 * B1 + A0 * B2);
                derD = A1 * B0 + A0 * B1;

                alpha = this->findRealRootFromCubic(derA, derB, derC, derD);

                chiEst += alpha * zeta;

                residuals = _forwardModel->calculateResidual(chiEst, pData);
                resSq = _forwardModel->calculateResidualNormSq(residuals);
                res = eta * resSq;

                std::cout << it1 + 1 << "/" << _cgInput.iteration1.n << "\t (" << it + 1 << "/" << _cgInput.n_max << ")\t res: "
                          << std::setprecision(17) << res << std::endl;

                file << std::setprecision(17) << res << "," << counter << std::endl;
                counter++;
                fDataOld = res;
                vecResFirstIter.push_back(res);

                if ((it1 > 0) && ((res < _cgInput.iteration1.tolerance) ||
                                  (std::abs(vecResFirstIter[it1 - 1] - res) < _cgInput.iteration1.tolerance)))
                {
                    bar.setCounter(_cgInput.iteration1.n + bar.getCounter() - (bar.getCounter() % _cgInput.iteration1.n));
                    break;
                }

                chiEst.Gradient(gradientChiOld);
                gradientChiNormsquared = (*gradientChiOld[0] * *gradientChiOld[0]) +
                                         (*gradientChiOld[1] * *gradientChiOld[1]); // why are there two elements in gradient Chi?
                                                                                    // how is the gradient calculated?

                tmpVolField = (gradientChiNormsquared + deltasquaredOld) / (gradientChiOldNormsquared + deltasquaredOld);
                fRegOld = (1.0 / (_grid.GetDomainArea())) *
                          tmpVolField.Summation() * _grid.GetCellVolume();

                deltasquaredOld = deltasquared;
                gOld = g;
                bsquaredOld = bsquared;
            }
            bar++;
        }
        // _forwardModel->calculatePTot(chiEst);
    }
    file.close();

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
    chiEst.CopyTo(result);
    return result;
}
