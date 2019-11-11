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

PressureFieldSerial ConjugateGradientInversion::calculateB(PressureFieldSerial** gradientChiNMin1)
{
    return bNSquared = 1.0 / (_grid.GetDomainArea() * calculateDeltaSq(gradientChiNMin1))
}

double ConjugateGradientInversion::calculateDeltaSquared(PressureFieldSerial** gradientChiNMin1)
{
    nTotal = _freq.nFreq * _src.nSrc * _recv.nRecv;
    double gradientChiNMin1NormSq = normSq(gradientChiNMin1, 0); 
    return 1.0 / _grid.GetDomainArea() * 
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

    const int nTotal = _freq.nFreq * _src.nSrc * _recv.nRecv;
    const double eta = 1.0 / (normSq(pData, nTotal));

    std::vector<std::complex<double>> residualsN;
    std::vector<std::complex<double>> kappaZetaN;
    std::vector<std::complex<double>> kappaZetaNMin1;

    PressureFieldSerial chiEstimateN(_grid);
    PressureFieldSerial chiEstimateNMin1(_grid);
    PressureFieldSerial zetaN(_grid);
    PressureFieldSerial zetaNMin1(_grid);
    PressureFieldSerial gN(_grid);
    PressureFieldSerial gNMin1(_grid);
    PressureFieldComplexSerial kappaStarResidualN(_grid);
    PressureFieldComplexSerial kappaStarResidualNMin1(_grid);
    PressureFieldSerial bN(_grid);
    PressureFieldSerial bNGradientChiN(_grid);
    PressureFieldSerial bNGradientChiNMin1(_grid);
    PressureFieldSerial gradientZetaN(_grid);
    PressureFieldSerial gradientZetaNMin1(_grid);
    PressureFieldSerial bNGradientZetaN(_grid);
    PressureFieldSerial bNGradientZetaNMin1(_grid);
    PressureFieldSerial gradientChiNormSquaredN(_grid);
    PressureFieldSerial gradientChiNormSquaredNMin1(_grid);
    PressureFieldSerial FRegIntegrand(_grid);

    PressureFieldSerial **gradientChiN = new PressureFieldSerial *[2];
    PressureFieldSerial **gradientChiNMin1 = new PressureFieldSerial *[2];
    
    double FDataN;
    double FDataNMin1;    
    double residualN;
    double gammaN;
    double alphaN;
    double deltaSquared;

    double A0;
    double A1;
    double A2;
    double B0;
    double B1;
    double B2;

    double ACardano;
    double BCardano;
    double CCardano;
    double DCardano;

    double deltaSquaredN;
    double deltaSquaredNMin1;

    chiEstimateN.Zero();

    for (int nOuterLoop = 0; nOuterLoop < _cgInput.n_max; nOuterLoop++)
    {
        _forwardModel->calculateKappa();
        residualsN = _forwardModel->calculateResidual(chiEstimateN, pData);
        for (int nInnerLoop = 0; nInnerLoop < _cgInput.iteration1.n; nInnerLoop++)
        {
            if (nInnerLoop == 0)
            {
                kappaStarResidualN = _forwardModel->calculateKappaConjugateResidual(residualsN);
                zetaN = eta * kappaStarResidualN.GetRealPart();
                alphaN = calculateAlpha(residualsN, zetaN);
                chiEstimateN += alphaN * zetaN;
                residualsN = _forwardModel->calculateResidual(chiEstimateN, pData);
                FDataN = eta * normSq(residualsN, 0);
            }
            else
            {
                A0 = FDataNMin1;
                A1 = 0.0;
                A2 = 0.0;
                kappaZetaN = _forwardModel->calculateKappaZeta(zetaN);
                for (int i = 0; i != nTotal; ++i)
                {
                    A1 += -2.0 * eta * std::real(conj(residualsN[i]) * kappaZetaN[i]);
                    A2 += eta * normSq(kappaZetaN, 0);
                }
                chiEstimateNMin1.Gradient(gradientChiNMin1);
                gradientChiNormSquaredN = *gradientChiNMin1[0] * *gradientChiNMin1[0] + *gradientChiNMin1[1] * *gradientChiNMin1[1];
                FRegIntegrand = normSq(chiGradientN, 0) / normSq(chiGradientNMin1, 0);
                bN = calculateB()
                B0 = normSq(bN * gradientChiNMin1, 0) + deltaSquaredNMin1 * normSq(bN, 0);
                B1 = 
                B2 = 
                ACardano = 4.0 * A2 * B2;
                BCardano = 3.0 * (A2 * B1 + A1 * B2);
                CCardano = 2.0 * (A2 * B0 + A1 * B1 + A0 * B2);
                DCardano = A1 * B0 + A0 * B1;
                alphaN = findRealRootFromCubic(ACardano, BCardano, CCardano, DCardano);
                chiEstimateN += alphaN * zetaN;
                residualsN = _forwardModel->calculateResidual(chiEstimateN, pData);
                FDataN = eta * normSq(residualsN, 0);
            }
            bar++;
        }
    }
    
    PressureFieldSerial result(_grid);
    chiEstimate.CopyTo(result);
    return result;
}
