#pragma once

#include "forwardModelContainer.h"
#include "forwardModelInterface.h"
#include "genericInput.h"
#include "gradientDescentInversionInput.h"
#include "inversionInterface.h"

class OpenMPGradientDescentInversion : public inversionInterface
{
private:
    ForwardModelContainer &_forwardModelsParallel;
    gradientDescentInversionInput _gdInput;

    const grid2D &_grid;
    const sources &_src;
    const receivers &_recv;
    const frequenciesGroup &_freq;

    dataGrid2D gradientDescent(dataGrid2D chiEstimate, const std::vector<double> &dfdx, double gamma);
    std::vector<double> differentialParallel(const std::vector<std::complex<double>> &pData, dataGrid2D xi, double dxi, double eta);

public:
    OpenMPGradientDescentInversion(const genericInput &gdInput, ForwardModelContainer &forwardmodels);

    OpenMPGradientDescentInversion(const OpenMPGradientDescentInversion &) = delete;
    OpenMPGradientDescentInversion &operator=(const OpenMPGradientDescentInversion &) = delete;

    dataGrid2D reconstruct(const std::vector<std::complex<double>> &pData, genericInput gInput);

    double functionFParallel(const dataGrid2D &xi, const std::vector<std::complex<double>> &pData, double eta);
    double determineGamma(const dataGrid2D chiEstimatePrevious, const dataGrid2D chiEstimateCurrent, std::vector<double> dFdxPrevious,
        std::vector<double> dFdxCurrent);
};
