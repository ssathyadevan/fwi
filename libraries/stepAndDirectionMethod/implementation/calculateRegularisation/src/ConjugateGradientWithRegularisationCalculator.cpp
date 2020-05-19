#include "ConjugateGradientWithRegularisationCalculator.h"

ConjugateGradientWithRegularisationCalculator::ConjugateGradientWithRegularisationCalculator(double errorFunctionalScalingFactor,
    forwardModelInterface *forwardModel, ConjugateGradientWithRegularisationParametersInput cgParametersInput, const std::vector<std::complex<double>> &pData) :
    DirectionCalculator(errorFunctionalScalingFactor, forwardModel),
    StepSizeCalculator(), _chiEstimatePrevious(forwardModel->getGrid()), _chiEstimateCurrent(forwardModel->getGrid()),
    _gradientPrevious(forwardModel->getGrid()), _gradientCurrent(forwardModel->getGrid()), _pData(pData), _cgParametersInput(cgParametersInput),
    _grid(forwardModel->getGrid()), _directionPrevious(forwardModel->getGrid()), _directionCurrent(forwardModel->getGrid()),
    _kappaTimesResidual(forwardModel->getGrid()), _regularisationPrevious(forwardModel->getGrid()), _regularisationCurrent(forwardModel->getGrid()),
    _residualVector(pData)
{
    _nTotal = forwardModel->getFreq().nFreq * forwardModel->getSrc().nSrc * forwardModel->getRecv().nRecv;
}

dataGrid2D &ConjugateGradientWithRegularisationCalculator::calculateDirection(const dataGrid2D &, const std::vector<std::complex<double>> &residualVector)
{
    // updating what necessary
    _residualVector = residualVector;
    _directionPrevious = _directionCurrent;
    _gradientPrevious = _gradientCurrent;

    _forwardModel->getUpdateDirectionInformation(_residualVector, _kappaTimesResidual);     // eq 2.13 first part, stored in _kappaTimesResidual
    _gradientCurrent = _errorFunctionalScalingFactor * _kappaTimesResidual.getRealPart();   // eq 2.13 second part

    double gamma = 0;
    if(_iterationNumber > 0)
    {
        gamma = calculateGammaPolakRibiere();   // eq 2.14
    }

    _directionCurrent = _gradientCurrent + gamma * _directionPrevious;
    return _directionCurrent;
}

void ConjugateGradientWithRegularisationCalculator::updateVariables(const dataGrid2D &chiEstimateCurrent, const dataGrid2D &, int iterationNumber,
    const std::vector<std::complex<double>> &, const std::vector<std::complex<double>> &)
{
    _iterationNumber = iterationNumber;

    _chiEstimatePrevious = _chiEstimateCurrent;
    _chiEstimateCurrent = chiEstimateCurrent;
}

double ConjugateGradientWithRegularisationCalculator::calculateStepSize() { return calculateRegularisationStep(); }

double ConjugateGradientWithRegularisationCalculator::calculateRegularisationStep()
{
    _forwardModel->calculateResidual(_chiEstimateCurrent, _pData);

    _deltaAmplification = _cgParametersInput._deltaAmplification._start / (_cgParametersInput._deltaAmplification._slope * _iterationNumber + 1.0);

    double alpha = 0.0;
    // regularisation loop
    for(int it = 0; it < _cgParametersInput._nRegularisationIterations; ++it)
    {
        calculateRegularisationParameters();

        _directionCurrent = calculateDirectionInRegularisation();   // eq: updateDirectionsCG
        alpha = calculateStepSizeRegularisation();

        // Update contrast-function
        _chiEstimateCurrent += alpha * _directionCurrent;

        // update _residualVector and _residualValueCurrent
        updateResidual();

        // break check for the inner loop
        if((it > 0) && ((_residualValueCurrent < _cgParametersInput._tolerance) ||
                           (std::abs(_residualValuePrevious - _residualValueCurrent) < _cgParametersInput._tolerance)))
        {
            break;
        }

        // Save variables for next iteration
        _residualValuePrevious = _residualValueCurrent;

        // Save regularisation variables for next iteration
        _chiEstimateCurrent.gradient(_regularisationCurrent.gradientChi);

        // this overwrites the vector _regularisationCurrent.gradientChi (first element for dx, second element for dz)
        calculateRegularisationErrorFunctional();

        _regularisationPrevious.deltaSquared = _regularisationCurrent.deltaSquared;
        _regularisationPrevious.bSquared = _regularisationCurrent.bSquared;
    }

    return alpha;
}

void ConjugateGradientWithRegularisationCalculator::calculateRegularisationParameters()
{
    // writes on _regularisationPrevious.gradientChi the gradient according to _chiEstimateCurrent
    _chiEstimateCurrent.gradient(_regularisationPrevious.gradientChi);

    _regularisationPrevious.gradientChiNormSquared =
        (_regularisationPrevious.gradientChi[0] * _regularisationPrevious.gradientChi[0]) +
        (_regularisationPrevious.gradientChi[1] * _regularisationPrevious.gradientChi[1]).summation();   // |dChi/dx^2 + dChi/dz^2|^2
    _regularisationCurrent.bSquared = calculateWeightingFactor();                                        //  eq. 2.22

    _regularisationCurrent.b = _regularisationCurrent.bSquared;
    _regularisationCurrent.b.sqrt();

    _regularisationCurrent.deltaSquared = calculateSteeringFactor();   // eq. 2.23

    _regularisationCurrent.gradient = calculateRegularisationGradient();
}

dataGrid2D ConjugateGradientWithRegularisationCalculator::calculateDirectionInRegularisation()
{
    _forwardModel->getUpdateDirectionInformation(_residualVector, _kappaTimesResidual);   // eq 2.13 _kappaTimesResidual updated
    _gradientCurrent =
        _errorFunctionalScalingFactor * _regularisationPrevious.errorFunctional * _kappaTimesResidual.getRealPart() +
        _residualValuePrevious *
            _regularisationCurrent.gradient;   // eq: 2.25 of thesis, bSquared is already included in .gradient, see calculateRegularisationGradient()

    double step = calculateGammaPolakRibiere();

    return _gradientCurrent + step * _directionCurrent;
}
dataGrid2D ConjugateGradientWithRegularisationCalculator::calculateWeightingFactor()
{
    dataGrid2D bSquared(_grid);
    bSquared = _regularisationPrevious.gradientChiNormSquared + _regularisationPrevious.deltaSquared;
    bSquared.reciprocal();
    bSquared *= (1.0 / (_grid.getDomainArea()));
    return bSquared;
}

double ConjugateGradientWithRegularisationCalculator::calculateSteeringFactor()
{
    //    return _regularisationPrevious.gradientChiNormSquared / (_grid.getDomainArea()); //according to the thesis

    dataGrid2D bTimesGradientChiXSquared = _regularisationCurrent.b * _regularisationPrevious.gradientChi[0];
    bTimesGradientChiXSquared.square();
    dataGrid2D bTimesGradientChiZSquared = _regularisationCurrent.b * _regularisationPrevious.gradientChi[1];
    bTimesGradientChiZSquared.square();

    double bTimesGradientChiNormSquared = (bTimesGradientChiXSquared + bTimesGradientChiZSquared).summation();
    double bSquaredSummed = _regularisationCurrent.bSquared.summation();

    // The _deltaAmplification * 0.5 part is not documented
    return _deltaAmplification * 0.5 * bTimesGradientChiNormSquared / bSquaredSummed;
}

dataGrid2D ConjugateGradientWithRegularisationCalculator::calculateRegularisationGradient()
{
    std::vector<dataGrid2D> temporaryGradient(2, dataGrid2D(_grid));

    dataGrid2D bSquaredTimesGradientChiX = _regularisationPrevious.bSquared * _regularisationPrevious.gradientChi[0];
    bSquaredTimesGradientChiX.gradient(temporaryGradient);

    dataGrid2D bSquaredTimesGradientChiZ = _regularisationPrevious.bSquared * _regularisationPrevious.gradientChi[1];
    bSquaredTimesGradientChiZ.gradient(temporaryGradient);

    dataGrid2D gradientBSquaredTimesGradientChiX = temporaryGradient[0];
    dataGrid2D gradientBSquaredTimesGradientChiZ = temporaryGradient[1];

    return gradientBSquaredTimesGradientChiX + gradientBSquaredTimesGradientChiZ;
}

double ConjugateGradientWithRegularisationCalculator::calculateGammaPolakRibiere()
{
    double gammaNumerator = 0.0;
    double gammaDenominator = 0.0;

    gammaNumerator = _gradientCurrent.innerProduct(_gradientCurrent - _gradientPrevious);
    gammaDenominator = _gradientPrevious.innerProduct(_gradientPrevious);

    if(gammaDenominator == 0.0)
    {
        throw std::overflow_error("ConjugateGradient: the computation of alpha devides by zero.");
    }
    return gammaNumerator / gammaDenominator;
}

void ConjugateGradientWithRegularisationCalculator::updateResidual()
{
    _residualVector = _forwardModel->calculateResidual(_chiEstimateCurrent, _pData);
    _residualValueCurrent = _errorFunctionalScalingFactor * _forwardModel->calculateResidualNormSq(_residualVector);
}

double ConjugateGradientWithRegularisationCalculator::calculateStepSizeRegularisation()
{
    std::vector<std::complex<double>> kappaTimesDirection(_nTotal);
    _forwardModel->mapDomainToSignal(_directionCurrent, kappaTimesDirection);

    double a0 = _residualValuePrevious;

    double a1 = 0.0;
    double a2 = 0.0;

    for(int i = 0; i < _nTotal; i++)
    {
        a1 += -2.0 * _errorFunctionalScalingFactor * std::real(conj(_residualVector[i]) * kappaTimesDirection[i]);
        a2 += _errorFunctionalScalingFactor * std::real(conj(kappaTimesDirection[i]) * kappaTimesDirection[i]);
    }

    dataGrid2D bGradientChiSquaredXDirection =
        (_regularisationCurrent.b * _regularisationPrevious.gradientChi[0]) * (_regularisationCurrent.b * _regularisationPrevious.gradientChi[0]);
    dataGrid2D bGradientChiSquaredZDirection =
        (_regularisationCurrent.b * _regularisationPrevious.gradientChi[1]) * (_regularisationCurrent.b * _regularisationPrevious.gradientChi[1]);
    double b0 = ((bGradientChiSquaredXDirection.summation() + bGradientChiSquaredZDirection.summation()) +
                    _regularisationPrevious.deltaSquared * _regularisationCurrent.bSquared.summation()) *
                _grid.getCellVolume();

    std::vector<dataGrid2D> gradientZeta(2, dataGrid2D(_grid));
    _directionCurrent.gradient(gradientZeta);

    dataGrid2D bGradientZetabGradientChiX = (_regularisationCurrent.b * gradientZeta[0]) * (_regularisationCurrent.b * _regularisationPrevious.gradientChi[0]);
    dataGrid2D bGradientZetabGradientChiZ = (_regularisationCurrent.b * gradientZeta[1]) * (_regularisationCurrent.b * _regularisationPrevious.gradientChi[1]);
    double b1 = 2.0 * (bGradientZetabGradientChiX.summation() + bGradientZetabGradientChiZ.summation()) * _grid.getCellVolume();

    dataGrid2D bTimesGradientZetaXdirection = _regularisationCurrent.b * gradientZeta[0];
    dataGrid2D bTimesGradientZetaZdirection = _regularisationCurrent.b * gradientZeta[1];
    bTimesGradientZetaXdirection.square();
    bTimesGradientZetaZdirection.square();
    double b2 = (bTimesGradientZetaXdirection.summation() + bTimesGradientZetaZdirection.summation()) * _grid.getCellVolume();

    // Parameters for the derivative of F^tot with respect to alpha, see eq. 2.26-2.27.
    // The final polynomial to solve is derA*x^3 + derB*x^2 + derC*x +derD = 0.

    double derA = 4.0 * a2 * b2;
    double derB = 3.0 * (a2 * b1 + a1 * b2);
    double derC = 2.0 * (a2 * b0 + a1 * b1 + a0 * b2);
    double derD = a1 * b0 + a0 * b1;

    // returning the only real solution of derA*x^3 + derB*x^2 + derC*x +derD = 0.
    return findRealRootFromCubic(derA, derB, derC, derD);
}

double ConjugateGradientWithRegularisationCalculator::findRealRootFromCubic(double a, double b, double c, double d)
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

void ConjugateGradientWithRegularisationCalculator::calculateRegularisationErrorFunctional()
{
    dataGrid2D gradientChiNormsquaredCurrent(_grid);
    gradientChiNormsquaredCurrent = (_regularisationCurrent.gradientChi[0] * _regularisationCurrent.gradientChi[0]) +
                                    (_regularisationCurrent.gradientChi[1] * _regularisationCurrent.gradientChi[1]);

    dataGrid2D integral = (gradientChiNormsquaredCurrent + _regularisationPrevious.deltaSquared) /
                          (_regularisationPrevious.gradientChiNormSquared + _regularisationPrevious.deltaSquared);

    _regularisationPrevious.errorFunctional = (1.0 / (_grid.getDomainArea())) * integral.summation() * _grid.getCellVolume();
}
