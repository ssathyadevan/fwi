#pragma once

#include "complexDataGrid2D.h"
#include "costFunction.h"
#include "dataGrid2D.h"
#include "frequenciesGroup.h"
#include "grid2D.h"
#include "log.h"
#include "receivers.h"
#include "sources.h"
#include <complex>

namespace fwi
{
    namespace forwardModels
    {
        inline double normSq(const std::vector<std::complex<double>> &data, int n)
        {
            double result = double(0.0);
            for(int i = 0; i < n; i++)
            {
                result += std::norm(data[i]);
            }

            return result;
        }

        class forwardModelInterface
        {
        public:
            forwardModelInterface() = default;

            virtual ~forwardModelInterface() = default;

            virtual const core::grid2D &getGrid() = 0;
            virtual const core::Sources &getSource() = 0;
            virtual const core::Receivers &getReceiver() = 0;
            virtual const core::FrequenciesGroup &getFreq() = 0;

            virtual CostFunction getCostFunction() = 0;

            virtual void calculatePData(const core::dataGrid2D &chiEst, std::vector<std::complex<double>> &pData) = 0;
            virtual void calculatePTot(const core::dataGrid2D &chiEst) = 0;
            virtual void mapDomainToSignal(const core::dataGrid2D &CurrentPressureFieldSerial, std::vector<std::complex<double>> &kOperator) = 0;

            virtual void calculateKappa() = 0;
            virtual void getUpdateDirectionInformation(const std::vector<std::complex<double>> &, core::complexDataGrid2D &) = 0;
            virtual void getUpdateDirectionInformationMPI(std::vector<std::complex<double>> &, core::complexDataGrid2D &, const int, const int) = 0;

            virtual std::vector<std::complex<double>> &calculateResidual(const core::dataGrid2D &chiEst, const std::vector<std::complex<double>> &pDataRef) = 0;
            virtual double calculateResidualNormSq(const std::vector<std::complex<double>> &residual) = 0;
            virtual double calculateCost(std::vector<std::complex<double>> &residualArray, core::dataGrid2D &chiEstimate,
                const std::vector<std::complex<double>> &pData, double eta) = 0;
            virtual double calculateLeastSquaresCost(std::vector<std::complex<double>> &residualArray, core::dataGrid2D &chiEstimate,
                const std::vector<std::complex<double>> &pData, double eta) = 0;
        };
    }   // namespace forwardModels
}   // namespace fwi
