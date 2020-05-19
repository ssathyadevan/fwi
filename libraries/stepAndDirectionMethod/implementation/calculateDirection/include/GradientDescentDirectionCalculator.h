#pragma once

#include "DirectionCalculator.h"

class GradientDescentDirectionCalculator : public DirectionCalculator
{
public:
    GradientDescentDirectionCalculator(
        double errorFunctionalScalingFactor, forwardModelInterface *forwardModel, double discretisationStep, const std::vector<std::complex<double>> &pData);
    virtual ~GradientDescentDirectionCalculator();

    /**
     * @brief calculateDirection for gradient descent method, computes the first derivative at every grid point taken singularly by means of adding the shift
     * _derivativeDiscretisationStep to one grid point, computing the derivative for that shift and then shift back to the original grid value.
     * @param chi is a dataGrid2D
     * @param residual is a vector of complex doubles (one value for each combination of sources, receivers and frequencies)
     * @return a dataGrid2D which represents the direction to follow
     */
    dataGrid2D &calculateDirection(const dataGrid2D &chiEstimate, const std::vector<std::complex<double>> &) override;

private:
    const std::vector<std::complex<double>> &_pData;
    const double _derivativeDiscretisationStep;

    /**
     * @brief optimizationFunction represents the function for which the derivative will be calculated
     * @param chiEstimate
     * @return a double which respresent the error which will be minimized
     */
    double optimizationFunction(const dataGrid2D &chiEstimate) const;
};
