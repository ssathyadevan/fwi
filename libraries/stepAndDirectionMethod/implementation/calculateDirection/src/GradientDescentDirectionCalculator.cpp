#include "GradientDescentDirectionCalculator.h"

GradientDescentDirectionCalculator::GradientDescentDirectionCalculator(
    double errorFunctionalScalingFactor, forwardModelInterface *forwardModel, double derivativeStepSize, const std::vector<std::complex<double>> &pData) :
    DirectionCalculator(errorFunctionalScalingFactor, forwardModel),
    _pData(pData), _derivativeStepSize(derivativeStepSize)
{
    if(derivativeStepSize <= 0.0)
    {
        throw std::invalid_argument("Derivative step size is zero or negative.");
    }
}

GradientDescentDirectionCalculator::~GradientDescentDirectionCalculator() {}

dataGrid2D GradientDescentDirectionCalculator::calculateDirection(const dataGrid2D &chiEstimate, const std::vector<std::complex<double>> &)
{
    dataGrid2D direction(chiEstimate.getGrid());
    dataGrid2D chiEstimatePlusH(chiEstimate);
    const int numberOfGridPoints = chiEstimate.getNumberOfGridPoints();

    const double errorChi = optimizationFunction(chiEstimate);
    double errorChiPlusH;
    double derivative;

    for(int i = 0; i < numberOfGridPoints; i++)
    {
        chiEstimatePlusH.addValueAtIndex(_derivativeStepSize, i);
        errorChiPlusH = optimizationFunction(chiEstimatePlusH);
        chiEstimatePlusH.addValueAtIndex(-_derivativeStepSize, i);

        derivative = (errorChiPlusH - errorChi) / _derivativeStepSize;
        direction.addValueAtIndex(derivative, i);
    }

    return direction;
}

double GradientDescentDirectionCalculator::optimizationFunction(const dataGrid2D &chiEstimate) const
{
    std::vector<std::complex<double>> residual = _forwardModel->calculateResidual(chiEstimate, _pData);
    const double currentChiError = _errorFunctionalScalingFactor * _forwardModel->calculateResidualNormSq(residual);
    return currentChiError;
}
