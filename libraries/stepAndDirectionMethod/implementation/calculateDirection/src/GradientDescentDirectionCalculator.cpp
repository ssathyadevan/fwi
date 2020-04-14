#include "GradientDescentDirectionCalculator.h"

GradientDescentDirectionCalculator::GradientDescentDirectionCalculator(
    double errorFunctionalScalingFactor, forwardModelInterface *forwardmodel, double derivativeStepSize, const std::vector<std::complex<double>> &pdata) :
    DirectionCalculator(errorFunctionalScalingFactor, forwardmodel),
    _pData(pdata), _derivativeStepSize(derivativeStepSize)
{
}

GradientDescentDirectionCalculator::~GradientDescentDirectionCalculator() {}

dataGrid2D GradientDescentDirectionCalculator::calculateDirection(const dataGrid2D &chiEstimate, const complexDataGrid2D &)
{
    dataGrid2D direction(chiEstimate.getGrid());
    dataGrid2D chiEstimatePlusH(chiEstimate);
    const int numberOfGridPoints = chiEstimate.getNumberOfGridPoints();

    double dFdx = 0.0;
    for(int i = 0; i < numberOfGridPoints; i++)
    {
        chiEstimatePlusH.addValueAtIndex(_derivativeStepSize, i);
        dFdx = optimizationFunction(chiEstimatePlusH);
        direction.addValueAtIndex(dFdx, i);
        chiEstimatePlusH.addValueAtIndex(-_derivativeStepSize, i);
    }

    return direction;
}

double GradientDescentDirectionCalculator::optimizationFunction(const dataGrid2D &chiEstimatePlusH) const
{
    std::vector<std::complex<double>> residual = _forwardmodel->calculateResidual(chiEstimatePlusH, _pData);
    const double dFdx = _errorFunctionalScalingFactor * _forwardmodel->calculateResidualNormSq(residual);
    return dFdx;
}
