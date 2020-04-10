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

    double dFdx;
    for(int i = 0; i < numberOfGridPoints; i++)
    {
        chiEstimatePlusH.addValueAtIndex(_derivativeStepSize, i);
        std::vector<std::complex<double>> residual = _forwardmodel->calculateResidual(chiEstimatePlusH, _pData);
        dFdx = _errorFunctionalScalingFactor * _forwardmodel->calculateResidualNormSq(residual);
        direction.addValueAtIndex(dFdx, i);
        chiEstimatePlusH.addValueAtIndex(-_derivativeStepSize, i);
    }

    return direction;
}

double GradientDescentDirectionCalculator::optimizationFunction() const { return 2.0; }

// double gradientDescentInversion::functionF(const dataGrid2D chiEstimate, const std::vector<std::complex<double>> &pData, double eta)
//{
//    std::vector<std::complex<double>> residual = _forwardModel->calculateResidual(chiEstimate, pData);
//    return eta * _forwardModel->calculateResidualNormSq(residual);
//}

// std::vector<double> gradientDescentInversion::differential(
//    const std::vector<std::complex<double>> &pData, dataGrid2D chiEstimate, double h, double eta)
//{
//    const int numGridPoints = chiEstimate.getNumberOfGridPoints();

//    double fx = functionF(chiEstimate, pData, eta);

//    double fxPlusH;
//    dataGrid2D chiEstimatePlusH(chiEstimate);
//    std::vector<double> dFdx(numGridPoints, 0.0);
//    for(int i = 0; i < numGridPoints; ++i)
//    {
//        chiEstimatePlusH.addValueAtIndex(h, i);   // Add h
//        fxPlusH = functionF(chiEstimatePlusH, pData, eta);
//        chiEstimatePlusH.addValueAtIndex(-h, i);   // Remove h

//        dFdx[i] = (fxPlusH - fx) / h;
//    }

//    return dFdx;
//}
