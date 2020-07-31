#pragma once

#include "CostFunctionCalculator.h"
#include "ForwardModelInterface.h"
#include "forwardModelContainer.h"
#include "genericInput.h"
#include "gradientDescentInversionInput.h"
#include "inversionInterface.h"
namespace fwi
{
    namespace inversionMethods
    {
        class OpenMPGradientDescentInversion : public inversionInterface
        {
        private:
            const core::CostFunctionCalculator &_costCalculator;
            ForwardModelContainer &_forwardModelsParallel;
            gradientDescentInversionInput _gdInput;

            const core::grid2D &_grid;
            const core::Sources &_source;
            const core::Receivers &_receiver;
            const core::FrequenciesGroup &_freq;

            core::dataGrid2D gradientDescent(core::dataGrid2D chiEstimate, const std::vector<double> &dfdx, double gamma);
            std::vector<double> differentialParallel(const std::vector<std::complex<double>> &pData, core::dataGrid2D xi, double dxi, double eta);
            std::ofstream openResidualLogFile(io::genericInput &gInput);

        public:
            OpenMPGradientDescentInversion(
                const core::CostFunctionCalculator &costCalculator, ForwardModelContainer &forwardmodels, const io::genericInput &gdInput);

            OpenMPGradientDescentInversion(const OpenMPGradientDescentInversion &) = delete;
            OpenMPGradientDescentInversion &operator=(const OpenMPGradientDescentInversion &) = delete;

            core::dataGrid2D reconstruct(const std::vector<std::complex<double>> &pData, io::genericInput gInput);

            double functionFParallel(const core::dataGrid2D &xi, const std::vector<std::complex<double>> &pData, double eta);
            double determineGamma(const core::dataGrid2D chiEstimatePrevious, const core::dataGrid2D chiEstimateCurrent, std::vector<double> dFdxPrevious,
                std::vector<double> dFdxCurrent);
        };
    }   // namespace inversionMethods
}   // namespace fwi
