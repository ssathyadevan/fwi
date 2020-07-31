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
            void calculateKappa() {}

            virtual std::vector<std::complex<double>> calculatePressureField(const core::dataGrid2D &chiEst);
            virtual void calculatePTot(const core::dataGrid2D &chiEst) { (void)chiEst; }
            virtual void getUpdateDirectionInformation(const std::vector<std::complex<double>> &, core::complexDataGrid2D &kappaTimesResidual)
            {
                kappaTimesResidual = _kappaTimesResidualValue;
            }   // the real KtimesResidual method cannot be applied without defining the whole core::complexDataGrid2D** _Kappa member, which defies the intent
                // of creating a mock class.
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
        };
    }   // namespace forwardModels
}   // namespace fwi
