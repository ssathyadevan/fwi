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
    double alphaNumerator = 0.0;
    double alphaDenominator = 0.0;

    std::vector<std::complex<double>> kappaTimesZeta(_nGridPoints);
    (void)kappaTimesZeta;
    //_forwardModel->mapDomainToSignal(zeta, kappaTimesZeta);
    // what is const dataGrid2D &zeta? Probably _chiEstimateCurrent
    for(int i = 0; i < _nGridPoints; i++)
    {
        // Also in need of a std::vector<std::complex<double>> &residualArray
        //        alphaNumerator += std::real(conj(residualArray[i]) * kappaTimesZeta[i]);
        //        alphaDenominator += std::real(conj(residualArray[i]) * kappaTimesZeta[i]);
    }

    if(alphaDenominator == 0.0)
    {
        throw std::overflow_error("ConjugateGradient: the computation of alpha devides by zero.");
    }
    double alpha = alphaNumerator / alphaDenominator;

    for(int i = 0; i < _cgParametersInput._nRegularisationIterations; ++i)
    {
        (void)alpha;
    }

    // applyRegularisation();

    // HERE INVOKE ALL THE REGULARISATION PROCESS, this will contain ALL the inner loop
    return 1.0;
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

dataGrid2D ConjugateGradientWithRegularisationCalculator::calculateDirection(
    const dataGrid2D &gradientCurrent, const std::vector<std::complex<double>> &residual)
{
    complexDataGrid2D kappaTimesResidual(_grid);   // eq: integrandForDiscreteK of README, KappaTimesResidual is the argument of Re()
    _forwardModel->getUpdateDirectionInformation(residual, kappaTimesResidual);
    (void)gradientCurrent;   // why this? it should be passed as const, and also in conjugateGradientInversion it doesn't seem like it is used, although it gets
                             // overwritten and invoked by reference

    return _errorFunctionalScalingFactor * kappaTimesResidual.getRealPart();
    ;
}
