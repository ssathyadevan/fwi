#pragma once

#include "forwardModelInterface.h"
#include "genericInput.h"
#include "gradientDescentInversionInput.h"
#include "inversionInterface.h"
class gradientDescentInversion : public inversionInterface
{
private:
    forwardModelInterface *_forwardModel;
    gradientDescentInversionInput _gdInput;

    const core::grid2D &_grid;
    const core::sources &_src;
    const core::receivers &_recv;
    const core::frequenciesGroup _freq;

    core::dataGrid2D gradientDescent(core::dataGrid2D chiEstimate, const std::vector<double> &dfdx, const double gamma);
    std::ofstream openResidualLogFile(genericInput &gInput);

public:
    gradientDescentInversion(forwardModelInterface *forwardModel, const gradientDescentInversionInput &gdInput);

    gradientDescentInversion(const gradientDescentInversion &) = delete;
    gradientDescentInversion &operator=(const gradientDescentInversion &) = delete;

    void logResidualResults(int iteration, double residual, bool isConverged);

    core::dataGrid2D reconstruct(const std::vector<std::complex<double>> &pData, genericInput gInput);
    std::vector<double> differential(
        std::vector<std::complex<double>> &residual, core::dataGrid2D xi, const std::vector<std::complex<double>> &pData, double eta, double dxi);
    double determineGamma(
        const core::dataGrid2D chiEstimatePrevious, const core::dataGrid2D chiEstimateCurrent, std::vector<double> dFdxPrevious, std::vector<double> dFdx);
};
