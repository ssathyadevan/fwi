#pragma once

#include "ForwardModelInterface.h"
#include "freqInfo.h"
#include "frequenciesGroup.h"
#include "greensFunctions.h"
#include "greensSerial.h"

namespace fwi
{
    namespace forwardModels
    {
        class ForwardModelMock : public ForwardModelInterface
        {
            double _kappaTimesResidualValue;

        public:
            ForwardModelMock(const core::grid2D &grid, const core::Sources &source, const core::Receivers &receiver, const core::FrequenciesGroup &freq);

            ~ForwardModelMock() {}
            const core::grid2D &getGrid();
            const core::Sources &getSource();
            const core::Receivers &getReceiver();
            const core::FrequenciesGroup &getFreq();
            CostFunction getCostFunction();
            void calculateKappa() {}

            virtual void calculatePData(const core::dataGrid2D &chiEst, std::vector<std::complex<double>> &pData);
            virtual void calculatePTot(const core::dataGrid2D &chiEst) { (void)chiEst; }
            virtual void getUpdateDirectionInformation(const std::vector<std::complex<double>> &, core::complexDataGrid2D &kappaTimesResidual)
            {
                kappaTimesResidual = _kappaTimesResidualValue;
            }   // the real KtimesResidual method cannot be applied without defining the whole core::complexDataGrid2D** _Kappa member, which defies the intent
                // of creating a mock class.
            virtual void mapDomainToSignal(const core::dataGrid2D &CurrentPressureFieldSerial, std::vector<std::complex<double>> &kOperator)
            {
                (void)CurrentPressureFieldSerial;
                (void)kOperator;
            }
            // to change value to kappaTimesResidual. Can only be invoked when creating a ForwardModelInterfaceMock* and not a ForwardModelInterface*
            void setKappaTimesResidualValue(double newKTRValue) { _kappaTimesResidualValue = newKTRValue; }
            std::vector<std::complex<double>> &calculateResidual(const core::dataGrid2D &chiEst, const std::vector<std::complex<double>> &pDataRef);
            double calculateResidualNormSq(const std::vector<std::complex<double>> &residual);
            double calculateCost(
                std::vector<std::complex<double>> &residualArray, core::dataGrid2D &chiEstimate, const std::vector<std::complex<double>> &pData, double eta);
            double calculateLeastSquaresCost(
                std::vector<std::complex<double>> &residualArray, core::dataGrid2D &chiEstimate, const std::vector<std::complex<double>> &pData, double eta);
            virtual void getUpdateDirectionInformationMPI(std::vector<std::complex<double>> &, core::complexDataGrid2D &, const int, const int)
            {
                L_(io::lerror) << "This ForwardModel is not compatible with the Inversion model";
                exit(EXIT_FAILURE);
            }

        private:
            std::vector<std::complex<double>> _residual;
            const core::grid2D &_grid;
            const core::Sources &_source;
            const core::Receivers &_receiver;
            const core::FrequenciesGroup &_freq;
            const CostFunction _costFunction;
        };
    }   // namespace forwardModels
}   // namespace fwi
