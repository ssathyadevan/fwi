#include "ConjugateGradientWithRegularisationCalculator.h"

// void RegularisationCalculator::calculateRegularisationParameters()
//{

//}

// dataGrid2D RegularisationCalculator::calculateUpdateRegularisationParameters()
//{

//}

// void RegularisationCalculator::calculateStepSizeRegularisation()
//{

//}

// const double RegularisationCalculator::errorFunctional()
//{

//}

// void RegularisationCalculator::logResidualResults()
//{

//}

ConjugateGradientWithRegularisationCalculator::ConjugateGradientWithRegularisationCalculator(double errorFunctionalScalingFactor,
    forwardModelInterface *forwardModel, ConjugateGradientWithRegularisationParametersInput cgParametersInput, const std::vector<std::complex<double>> &pData) :
    DirectionCalculator(errorFunctionalScalingFactor, forwardModel),
    _chiEstimatePrevious(forwardModel->getGrid()), _chiEstimateCurrent(forwardModel->getGrid()), _derivativePrevious(forwardModel->getGrid()),
    _derivativeCurrent(forwardModel->getGrid()), _pData(pData), _cgParametersInput(cgParametersInput), _grid(forwardModel->getGrid()),
    _sources(forwardModel->getSrc()), _receivers(forwardModel->getRecv()), _frequencies(forwardModel->getFreq())
{
    _nGridPoints = _grid.getNumberOfGridPoints();
}

double ConjugateGradientWithRegularisationCalculator::calculateStepSize()
{
    double stepNumerator = 0.0;
    double stepDenominator = 0.0;

    std::vector<double> derivativeCurrentData = _derivativeCurrent.getData();
    std::vector<double> derivativePreviousData = _derivativePrevious.getData();

    std::vector<std::complex<double>> kappaTimesZeta(_nGridPoints);
    (void)kappaTimesZeta;
    //_forwardModel->mapDomainToSignal(zeta, kappaTimesZeta);
    // what is const dataGrid2D &zeta? Probably _chiEstimateCurrent
    for(int i = 0; i < _nGridPoints; i++)
    {
        // by integrating the equations (integrandForDiscreteK) and (PolakRibiereDirection) with some help from wikipedia (so making clear that g(x) is the
        // gradient/derivative, we get Also in need of a std::vector<std::complex<double>> &residualArray
        stepNumerator += derivativeCurrentData[i] * (derivativeCurrentData[i] - derivativePreviousData[i]);
        stepDenominator += std::pow(derivativePreviousData[i], 2);
    }

    if(stepDenominator == 0.0)
    {
        throw std::overflow_error("ConjugateGradient: the computation of alpha devides by zero.");
    }
    double step = stepNumerator / stepDenominator;

    for(int i = 0; i < _cgParametersInput._nRegularisationIterations; ++i)
    {
        (void)step;
    }

    // applyRegularisation();

    // HERE INVOKE ALL THE REGULARISATION PROCESS, this will contain ALL the inner loop
    return 1.0;
}

// in the Inversion this is calculateUpdateDirection
dataGrid2D ConjugateGradientWithRegularisationCalculator::calculateDirection(const dataGrid2D &, const std::vector<std::complex<double>> &residual)
{   // the dataGrid2D gradientCurrent is not used here, as it cannot be overwritten

    complexDataGrid2D kappaTimesResidual(_grid);   // eq: integrandForDiscreteK of README, KappaTimesResidual is the argument of Re()
    _forwardModel->getUpdateDirectionInformation(residual, kappaTimesResidual);

    return _errorFunctionalScalingFactor * kappaTimesResidual.getRealPart();
}

void ConjugateGradientWithRegularisationCalculator::updateVariables(
    const dataGrid2D &chiEstimateCurrent, const dataGrid2D &derivativeCurrent, int iterationNumber)
{
    _iterationNumber = iterationNumber;

    _chiEstimatePrevious = _chiEstimateCurrent;
    _derivativePrevious = _derivativeCurrent;
    _chiEstimateCurrent = chiEstimateCurrent;
    _derivativeCurrent = derivativeCurrent;
}
