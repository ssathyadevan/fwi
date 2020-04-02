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

    const grid2D &_grid;
    const sources &_src;
    const receivers &_recv;
    const frequenciesGroup _freq;

    dataGrid2D gradientDescent(dataGrid2D chiEstimate, const std::vector<double> &dfdx, const double gamma);

public:
    gradientDescentInversion(forwardModelInterface *forwardModel, const gradientDescentInversionInput &gdInput);

    gradientDescentInversion(const gradientDescentInversion &) = delete;
    gradientDescentInversion &operator=(const gradientDescentInversion &) = delete;

    dataGrid2D reconstruct(const std::vector<std::complex<double>> &pData, genericInput gInput);
    std::vector<double> differential(const std::vector<std::complex<double>> &pData, dataGrid2D xi, double dxi, double eta);
    double functionF(const dataGrid2D xi, const std::vector<std::complex<double>> &pData, double eta);
    double determineGamma(const dataGrid2D chiEstimatePrevious, const dataGrid2D chiEstimateCurrent, std::vector<double> dFdxPrevious,
        std::vector<double> dFdx);
};
