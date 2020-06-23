#include "GradientDescentDirectionCalculator.h"

GradientDescentDirectionCalculator::GradientDescentDirectionCalculator(
    double errorFunctionalScalingFactor, forwardModels::forwardModelInterface *forwardModel, double discretisationStep, const std::vector<std::complex<double>> &pData) :
    DirectionCalculator(errorFunctionalScalingFactor, forwardModel),
    _pData(pData), _derivativeDiscretisationStep(discretisationStep)
{
    if(discretisationStep <= 0.0)
    {
        throw std::invalid_argument("Derivative step size is zero or negative.");
    }
    _direction.zero();
}

GradientDescentDirectionCalculator::~GradientDescentDirectionCalculator() {}

core::dataGrid2D &GradientDescentDirectionCalculator::calculateDirection(const core::dataGrid2D &chiEstimate, const std::vector<std::complex<double>> &)
{
    //   _direction.zero();
    core::dataGrid2D chiEstimatePlusH(chiEstimate);
    const int numberOfGridPoints = chiEstimate.getNumberOfGridPoints();

    const double errorChi = optimizationFunction(chiEstimate);
    double errorChiPlusH;
    double derivative;

    for(int i = 0; i < numberOfGridPoints; ++i)
    {
        // for the i-th variable/grid point we compute the approximate derivative by computing
        // D_i F(chiEstimate) ~= (F(chiEstimate +e_i*_derivativeDiscretisationStep) - F(chiEstimate))/_derivativeDiscretisationStep.
        // In literature the variable _derivativeDiscretisationStep is most often called 'h/H'.
        // As for 'e_i' we mean a mathematical vector pointing towards the i-th direction/variable/grid point with unitary length, so that adding it to another
        // vector only modifies the i-th entry.
        chiEstimatePlusH.addValueAtIndex(_derivativeDiscretisationStep, i);
        errorChiPlusH = optimizationFunction(chiEstimatePlusH);
        chiEstimatePlusH.addValueAtIndex(-_derivativeDiscretisationStep, i);

        derivative = (errorChiPlusH - errorChi) / _derivativeDiscretisationStep;
        _direction.setValueAtIndex(-derivative, i);   // the - sign means we we can sum _direction = - gradient
    }

    return _direction;
}

double GradientDescentDirectionCalculator::optimizationFunction(const core::dataGrid2D &chiEstimate) const
{
    std::vector<std::complex<double>> residual = _forwardModel->calculateResidual(chiEstimate, _pData);
    const double currentChiError = _errorFunctionalScalingFactor * _forwardModel->calculateResidualNormSq(residual);
    return currentChiError;
}
